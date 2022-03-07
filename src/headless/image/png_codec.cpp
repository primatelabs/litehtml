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

#include "image/png_codec.h"

#include <assert.h>

#include <algorithm>

#include "png.h"

#include "math/clamp.h"
#include "litehtml/logging.h"

namespace headless {

namespace {

struct PNGBuffer {
  PNGBuffer(uint8_t* d, uint32_t l)
  : data(d)
  , length(l)
  , offset(0)
  {}

  uint8_t* data;
  uint32_t length;
  uint32_t offset;
};


void png_error_fn(png_structp png_ptr, png_const_charp error_msg)
{
  LOG(ERROR) << error_msg;
}


void png_warning_fn(png_structp png_ptr, png_const_charp warning_msg)
{
  LOG(INFO) << warning_msg;
}


void png_read_data(png_structp png, png_bytep data, png_size_t length)
{
  PNGBuffer* buffer = reinterpret_cast<PNGBuffer*>(png_get_io_ptr(png));

  assert(buffer->length >= (buffer->offset + length));

  memcpy(data, buffer->data + buffer->offset, length);
  buffer->offset += length;
}


void png_write_data(png_structp png, png_bytep data, png_size_t length)
{
  PNGBuffer* buffer = reinterpret_cast<PNGBuffer*>(png_get_io_ptr(png));

  uint32_t required_length = buffer->offset + length;
  if (buffer->length < required_length) {
    uint32_t realloc_length = std::max(buffer->length * 2, required_length * 2);
    buffer->data = static_cast<uint8_t*>(realloc(buffer->data, realloc_length));
    buffer->length = realloc_length;
  }

  assert(buffer->length >= (buffer->offset + length));

  memcpy(buffer->data + buffer->offset, data, length);
  buffer->offset += length;
}

void png_flush_data(png_structp)
{
}

void png_grayscale_to_rgb(uint8_t* src, uint8_t* dst, int width)
{
  constexpr int src_channels = 1;
  constexpr int dst_channels = 3;
  for (int i = 0; i < width; i++) {
    dst[i * dst_channels + 0] = src[i * src_channels + 0];
    dst[i * dst_channels + 1] = src[i * src_channels + 0];
    dst[i * dst_channels + 2] = src[i * src_channels + 0];
  }
}

void png_grayscale_to_rgba(uint8_t* src, uint8_t* dst, int width)
{
  constexpr int src_channels = 1;
  constexpr int dst_channels = 4;
  for (int i = 0; i < width; i++) {
    dst[i * dst_channels + 0] = src[i * src_channels + 0];
    dst[i * dst_channels + 1] = src[i * src_channels + 0];
    dst[i * dst_channels + 2] = src[i * src_channels + 0];
    dst[i * dst_channels + 3] = 255;
  }
}

void png_grayscale_alpha_to_rgba(uint8_t* src, uint8_t* dst, int width)
{
  constexpr int src_channels = 2;
  constexpr int dst_channels = 4;
  for (int i = 0; i < width; i++) {
    dst[i * dst_channels + 0] = src[i * src_channels + 0];
    dst[i * dst_channels + 1] = src[i * src_channels + 0];
    dst[i * dst_channels + 2] = src[i * src_channels + 0];
    dst[i * dst_channels + 3] = src[i * src_channels + 1];
  }
}

void png_rgb_to_grayscale(uint8_t* src, uint8_t* dst, int width)
{
  constexpr int src_channels = 3;
  constexpr int dst_channels = 1;
  for (int i = 0; i < width; i++) {
    int r = src[i * src_channels + 0];
    int g = src[i * src_channels + 1];
    int b = src[i * src_channels + 2];
    int gray = (9798 * r + 19235 * g + 3735 * b) / 32768;
    dst[i * dst_channels] = clamp(gray, 0, 255);

  }
}

void png_rgb_to_rgba(uint8_t* src, uint8_t* dst, int width)
{
  constexpr int src_channels = 3;
  constexpr int dst_channels = 4;
  for (int i = 0; i < width; i++) {
    dst[i * dst_channels + 0] = src[i * src_channels + 0];
    dst[i * dst_channels + 1] = src[i * src_channels + 1];
    dst[i * dst_channels + 2] = src[i * src_channels + 2];
    dst[i * dst_channels + 3] = 255;
  }
}

void png_rgba_to_grayscale(uint8_t* src, uint8_t* dst, int width)
{
  constexpr int src_channels = 4;
  constexpr int dst_channels = 1;
  for (int i = 0; i < width; i++) {
    int r = src[i * src_channels + 0];
    int g = src[i * src_channels + 1];
    int b = src[i * src_channels + 2];
    int gray = (9798 * r + 19235 * g + 3735 * b) / 32768;
    dst[i * dst_channels] = clamp(gray, 0, 255);

  }
}

void png_rgba_to_rgb(uint8_t* src, uint8_t* dst, int width)
{
  constexpr int src_channels = 4;
  constexpr int dst_channels = 3;
  for (int i = 0; i < width; i++) {
    dst[i * dst_channels + 0] = src[i * src_channels + 0];
    dst[i * dst_channels + 1] = src[i * src_channels + 1];
    dst[i * dst_channels + 2] = src[i * src_channels + 2];
  }
}

} // namespace

#define PNG_CONVERT(convert_fn) \
  for (int j = 0; j < height; j++) {          \
    png_read_row(png_ptr, row_pointer, NULL); \
    uint8_t* dst_row = image.row(j);          \
    convert_fn(row_pointer, dst_row, width);  \
  }

#define PNG_UNHANDLED(img_format) \
  {                                                     \
    std::ostringstream oss;                             \
    oss << "PNGCodec: Unhandled format " << img_format; \
    throw std::runtime_error(oss.str());                \
  }

Image<uint8_t> PNGCodec::decompress(uint8_t* data, size_t length, ImageFormat format)
{
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  png_infop info_ptr = png_create_info_struct(png_ptr);

  if (!png_ptr || !info_ptr) {
    // TODO: Throw an exception.
  }

  // Override libpng's default error and warning functions.
  png_set_error_fn(png_ptr, nullptr, png_error_fn, png_warning_fn);

  PNGBuffer buffer(data, length);
  png_set_read_fn(png_ptr, &buffer, png_read_data);

  png_read_info(png_ptr, info_ptr);

  png_uint_32 width;
  png_uint_32 height;
  int bit_depth;
  int color_type;
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, nullptr, nullptr, nullptr);
  int channels = png_get_channels(png_ptr, info_ptr);

  png_read_update_info(png_ptr, info_ptr);


  ImageFormat src_format = channels_format(channels);

  if (format == kImageFormatDefault) {
    format = src_format;
  }

  Image<uint8_t> image(width, height, format);

  if (src_format == format) {
    // This is the straightforward case! Since both formats are the same we
    // can use png_read_image() to read the entire image at once.  There's no
    // need to load the image row-by-row to convert between formats.

    png_bytep* row_pointers = static_cast<png_bytep*>(malloc(height * sizeof(png_bytep)));
    for (int i = 0; i < height; i++) {
      row_pointers[i] = image.row(i);
    }
    png_read_image(png_ptr, row_pointers);
    free(row_pointers);
  } else {
    // This is the, well, less straightforward case! Since the source and
    // destination formats are different we read the image row-by-row and
    // convert the image from the source format to the destination format.

    png_bytep row_pointer = static_cast<png_bytep>(malloc(width * sizeof(png_byte) * channels));

    LOG(INFO) << src_format << " " << format;

    if (src_format == kImageFormatGrayscale) {
      if (format == kImageFormatRGB) {
        PNG_CONVERT(png_grayscale_to_rgb)
      } else if (format == kImageFormatRGBA) {
        PNG_CONVERT(png_grayscale_to_rgba)
      } else {
        PNG_UNHANDLED(format);
      }
    } else if (src_format == kImageFormatGrayscaleAlpha) {
      if (format == kImageFormatRGBA) {
        PNG_CONVERT(png_grayscale_alpha_to_rgba)
      } else {
        PNG_UNHANDLED(format);
      }
    } else if (src_format == kImageFormatRGB) {
      if (format == kImageFormatGrayscale) {
        PNG_CONVERT(png_rgb_to_grayscale)
      } else if (format == kImageFormatRGBA) {
        PNG_CONVERT(png_rgb_to_rgba)
      } else {
        PNG_UNHANDLED(format);
      }
    } else if (src_format == kImageFormatRGBA) {
      if (format == kImageFormatGrayscale) {
        PNG_CONVERT(png_rgba_to_grayscale)
      } else if (format == kImageFormatRGB) {
        PNG_CONVERT(png_rgba_to_rgb)
      } else {
        PNG_UNHANDLED(format);
      }
    } else {
      PNG_UNHANDLED(src_format);
    }

    free(row_pointer);
  }
  png_read_end(png_ptr, info_ptr);
  png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

  return image;
}

void PNGCodec::compress(Image<uint8_t>& image, uint8_t** data, size_t* length)
{
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  png_infop info_ptr = png_create_info_struct(png_ptr);

  if (!png_ptr || !info_ptr) {
    // TODO: Throw an exception.
  }

  // Override libpng's default error and warning functions.
  png_set_error_fn(png_ptr, nullptr, png_error_fn, png_warning_fn);

  // Heuristic to guess the amount of memory to allocate upfront (we'd like to
  // avoid reallocating memory).  Assume that the PNG compression ratio is
  // approximately 2:1, so that the PNG data should fit inside a buffer half
  // the size of the raw pixel data.  This assumption falls apart for small
  // images, so set a minimum size of 64KB.
  //
  // TODO: Allow the caller to provide the expected size of the compressed
  // data so we can avoid over-allocating when using this function as part of
  // a workload.

  *length = image.width() * image.height() * image.channels() / 2;
  *length = std::max(static_cast<size_t>(65536), *length);
  *data = static_cast<uint8_t*>(malloc(*length));

  PNGBuffer buffer(*data, *length);
  png_set_write_fn(png_ptr, &buffer, png_write_data, png_flush_data);

  int bit_depth = 8;
  int color_type = PNG_COLOR_TYPE_RGB;

  switch (image.channels()) {
    case 1:
      color_type = PNG_COLOR_TYPE_GRAY;
      break;

    case 2:
      color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
      break;

    case 3:
      color_type = PNG_COLOR_TYPE_RGB;
      break;

    case 4:
      color_type = PNG_COLOR_TYPE_RGB_ALPHA;
      break;

    default:
      assert(false);
  }

  png_set_IHDR(png_ptr, info_ptr, image.width(), image.height(), bit_depth, color_type,
    PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(png_ptr, info_ptr);

  png_bytep* row_pointers = static_cast<png_bytep*>(malloc(image.height() * sizeof(png_bytep)));
  for (uint32_t i = 0; i < image.height(); i++) {
    row_pointers[i] = image.row(i);
  }
  png_write_image(png_ptr, row_pointers);
  free(row_pointers);

  png_write_end(png_ptr, info_ptr);
  png_destroy_write_struct(&png_ptr, &info_ptr);

  *length = buffer.offset;
  *data = buffer.data;
}

} // namespace headless
