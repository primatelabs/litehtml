// Copyright (C) 2020-2021 Primate Labs Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the names of the copyright holders nor the names of their
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "image/jpeg_codec.h"

#include <assert.h>

#include <algorithm>

#include "math/clamp.h"
#include "litehtml/logging.h"

// HACK: Work around macro redefinitions in jmorecfh.h.
#if defined(PLATFORM_WINDOWS)
#define XMD_H
#endif

extern "C" {
  #include "jpeglib.h"
  #include "jerror.h"
}

// HACK: Work around macro redefinitions in jmorecfh.h.
#if defined(PLATFORM_WINDOWS)
#undef XMD_H
#endif

namespace headless {

namespace {

struct MemorySource {
  jpeg_source_mgr pub;
  uint8_t* data;
  uint32_t length;
  uint32_t offset;
};


void init_source(j_decompress_ptr cinfo)
{
  MemorySource* src = reinterpret_cast<MemorySource*>(cinfo->src);
  src->pub.bytes_in_buffer = 0;
}


boolean fill_input_buffer(j_decompress_ptr cinfo)
{
  MemorySource* src = reinterpret_cast<MemorySource*>(cinfo->src);
  static const uint8_t eof[] = { 0xFF, JPEG_EOI };

  if (src->offset >= src->length) {
    WARNMS(cinfo, JWRN_JPEG_EOF);
    src->pub.next_input_byte = eof;
    src->pub.bytes_in_buffer = 2;
  } else {
    src->pub.next_input_byte = src->data + src->offset;
    src->pub.bytes_in_buffer = src->length - src->offset;
    src->offset = src->length;
  }

  return TRUE;
}


void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
  MemorySource* src = reinterpret_cast<MemorySource*>(cinfo->src);

  if (num_bytes > 0) {
    while (num_bytes > static_cast<long>(src->pub.bytes_in_buffer)) {
      num_bytes -= src->pub.bytes_in_buffer;
      fill_input_buffer(cinfo);
    }
    src->pub.next_input_byte += num_bytes;
    src->pub.bytes_in_buffer -= num_bytes;
  }
}


void term_source(j_decompress_ptr cinfo)
{
}


void jpeg_memory_source(j_decompress_ptr cinfo, uint8_t* data, size_t length)
{
  if(cinfo->src == nullptr) {
    cinfo->src = (jpeg_source_mgr*)(*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT, sizeof(MemorySource));
  }

  MemorySource* src = reinterpret_cast<MemorySource*>(cinfo->src);
  src->pub.init_source = init_source;
  src->pub.fill_input_buffer = fill_input_buffer;
  src->pub.skip_input_data = skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart;
  src->pub.term_source = term_source;
  src->length = length;
  src->offset = 0;
  src->data = data;
}


struct MemoryDestination {
  jpeg_destination_mgr pub;
  uint32_t length;
  uint32_t offset;
  uint8_t* data;
};


void init_destination(j_compress_ptr cinfo)
{
  MemoryDestination* dest = reinterpret_cast<MemoryDestination*>(cinfo->dest);

  dest->pub.next_output_byte = dest->data;
  dest->pub.free_in_buffer = dest->length;
}


boolean empty_output_buffer(j_compress_ptr cinfo)
{
  MemoryDestination* dest = reinterpret_cast<MemoryDestination*>(cinfo->dest);

  // Double the length of the buffer; this should minimize the total number of
  // reallocations needed at the expense of overallocating memory.  With the
  // heuristic in JPEGCodec::compress() we should never need to reallocate
  // memory, but if we do the least we can do is be smart about it.
  uint32_t realloc_length = dest->length * 2;
  dest->data = static_cast<uint8_t*>(realloc(dest->data, realloc_length));
  dest->pub.next_output_byte = dest->data + dest->length;
  dest->pub.free_in_buffer = realloc_length - dest->length;
  dest->length = realloc_length;

  return TRUE;
}

void term_destination(j_compress_ptr cinfo)
{
  MemoryDestination* dest = reinterpret_cast<MemoryDestination*>(cinfo->dest);

  dest->offset = dest->length - dest->pub.free_in_buffer;
}

void jpeg_memory_destination(j_compress_ptr cinfo, uint8_t * data, uint32_t length)
{
  MemoryDestination* dest;

  if(cinfo->dest == nullptr) {
    cinfo->dest = (jpeg_destination_mgr *)(*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT, sizeof(MemoryDestination));
  }

  dest = reinterpret_cast<MemoryDestination*>(cinfo->dest);
  dest->pub.init_destination = init_destination;
  dest->pub.empty_output_buffer = empty_output_buffer;
  dest->pub.term_destination = term_destination;
  dest->length = length;
  dest->offset = 0;
  dest->data = data;
}

} // namespace

Image<uint8_t> JPEGCodec::decompress(uint8_t* data, size_t length, ImageFormat format)
{
  jpeg_decompress_struct cinfo;
  jpeg_error_mgr jerr;
  JSAMPROW row;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);

  jpeg_memory_source(&cinfo, data, length);
  jpeg_read_header(&cinfo, TRUE);

  // RGB is the default format for JPEG images because we assume all JPEG
  // images are RGB images (see the assertion above).
  if (format == kImageFormatDefault) {
    format = kImageFormatRGB;
  }

  // Decompress JPEG images to a temporary RGB buffer then convert to RGBA or
  // grayscale ourselves when the target image format is not RGB.

  switch (format) {
    case kImageFormatRGB:
    case kImageFormatRGBA:
    case kImageFormatGrayscale:
    default:
      cinfo.out_color_space = JCS_RGB;
      break;
  }

  jpeg_start_decompress(&cinfo);

  int row_stride = cinfo.output_width * cinfo.output_components;
  JSAMPARRAY row_buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

  int width = cinfo.output_width;
  int height = cinfo.output_height;
  int channels = cinfo.output_components;

  Image<uint8_t> image(width, height, format);

  if (format == kImageFormatRGB) {
    // Thanks to the libjpeg-turbo colorspace extensions we can just copy the
    // decompressed image from the row buffer to the image without performing
    // any conversion.

    while (cinfo.output_scanline < cinfo.output_height) {
      jpeg_read_scanlines(&cinfo, row_buffer, 1);
      row = row_buffer[0];
      memcpy(image.row(cinfo.output_scanline - 1), row, width * channels);
    }
  } else if (format == kImageFormatRGBA) {
    while (cinfo.output_scanline < cinfo.output_height) {
      jpeg_read_scanlines(&cinfo, row_buffer, 1);
      JSAMPROW row = row_buffer[0];
      uint8_t* dst_row = image.row(cinfo.output_scanline - 1);
      for (int i = 0; i < width; i++) {
        dst_row[i * 4 + 0] = *row++;
        dst_row[i * 4 + 1] = *row++;
        dst_row[i * 4 + 2] = *row++;
        dst_row[i * 4 + 3] = 255;
      }
    }
  } else if (format == kImageFormatGrayscale) {
    // Convert the pixels from RGB to grayscale while copying the decompressed
    // image from the row buffer to the image.

    while (cinfo.output_scanline < cinfo.output_height) {
      jpeg_read_scanlines(&cinfo, row_buffer, 1);
      JSAMPROW row = row_buffer[0];
      uint8_t* dst_row = image.row(cinfo.output_scanline - 1);
      for (int i = 0; i < width; i++) {
        int r = *row++;
        int g = *row++;
        int b = *row++;
        int gray = (9798 * r + 19235 * g + 3735 * b) / 32768;
        dst_row[i] = clamp(gray, 0, 255);
      }
    }
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  return image;
}

void JPEGCodec::compress(Image<uint8_t>& image, uint8_t** data, size_t* length, int quality)
{
  jpeg_compress_struct cinfo;
  jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);

  // Heuristic to guess the amount of memory to allocate upfront (we'd like to
  // avoid reallocating memory).  Assume that the JPEG compression ratio is
  // approximately 10:1, so that the JPEG data should fit inside a buffer 1/10
  // the size of the raw pixel data.  This assumption falls apart for small
  // images, so set a minimum size of 64KB.
  //
  // TODO: Allow the caller to provide the expected size of the compressed
  // data so we can avoid over-allocating when using this function as part of
  // a workload.

  *length = image.width() * image.height() * image.channels() / 10;
  *length = std::max(static_cast<size_t>(65536), *length);
  *data = static_cast<uint8_t*>(malloc(*length));

  jpeg_memory_destination(&cinfo, *data, *length);
  cinfo.image_width = image.width();
  cinfo.image_height = image.height();
  cinfo.input_components = image.channels();

  // We currently only support compressing RGB images to JPEG.
  switch (image.format()) {
    case kImageFormatRGB:
      cinfo.in_color_space = JCS_RGB;
      break;

    default:
      assert(false);
      break;
  }

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE);
  jpeg_start_compress(&cinfo, TRUE);
  while(cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = image.row(cinfo.next_scanline);
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }
  jpeg_finish_compress(&cinfo);

  MemoryDestination* dest = reinterpret_cast<MemoryDestination*>(cinfo.dest);
  *data = dest->data;
  *length = dest->offset;

  jpeg_destroy_compress(&cinfo);
}

} // namespace headless
