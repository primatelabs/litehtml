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

#include "image/convert.h"

#include <assert.h>

#include <stdexcept>

#include "litehtml/logging.h"
#include "math/clamp.h"

namespace headless {

namespace {

void convert_grayscale(const Image<uint8_t>& src, Image<uint8_t>& dst)
{
  assert(src.format() != kImageFormatGrayscale);
  assert(dst.format() == kImageFormatGrayscale);

  for (int y = 0; y < src.height(); y++) {
    const uint8_t* src_pixel = src.row(y);
    uint8_t* dst_pixel = dst.row(y);
    for (int x = 0; x < src.width(); x++) {
      int r = src_pixel[0];
      int g = src_pixel[1];
      int b = src_pixel[2];

      // The constants are from the ITU-R BT.601 standard used for HDTV.
      int gray = (9798 * r + 19235 * g + 3735 * b) / 32768;
      dst_pixel[0] = clamp(gray, 0, 255);

      src_pixel += src.channels();
      dst_pixel += 1;
    }
  }
}

void convert_rgb(const Image<uint8_t>& src, Image<uint8_t>& dst)
{
  assert(src.format() != kImageFormatRGB);
  assert(dst.format() == kImageFormatRGB);

  // Map source image channels to destination image channels.
  int c0 = 0;
  int c1 = 1;
  int c2 = 2;

  if (src.channels() == 1) {
    c0 = 0;
    c1 = 0;
    c2 = 0;
  }

  for (size_t i = 0; i < src.height(); i++) {
    const uint8_t* src_pixel = src.row(i);
    uint8_t* dst_pixel = dst.row(i);
    for (size_t j = 0; j < src.width(); j++) {
      dst_pixel[0] = src_pixel[c0];
      dst_pixel[1] = src_pixel[c1];
      dst_pixel[2] = src_pixel[c2];

      src_pixel += src.channels();
      dst_pixel += 3;
    }
  }
}

void convert_rgba(const Image<uint8_t>& src, Image<uint8_t>& dst)
{
  assert(src.format() != kImageFormatRGBA);
  assert(dst.format() == kImageFormatRGBA);

  // Map source image channels to destination image channels.
  int c0 = 0;
  int c1 = 1;
  int c2 = 2;

  if (src.channels() == 1) {
    c0 = 0;
    c1 = 0;
    c2 = 0;
  }

  for (size_t i = 0; i < src.height(); i++) {
    const uint8_t* src_pixel = src.row(i);
    uint8_t* dst_pixel = dst.row(i);
    for (size_t j = 0; j < src.width(); j++) {
      dst_pixel[0] = src_pixel[c0];
      dst_pixel[1] = src_pixel[c1];
      dst_pixel[2] = src_pixel[c2];
      dst_pixel[3] = 255;

      src_pixel += src.channels();
      dst_pixel += 4;
    }
  }
}

} // namespace

Image<uint8_t> convert(const Image<uint8_t>& src, ImageFormat format)
{
  if (src.format() == format) {
    LOG(WARNING) << "convert() called on an image with the same format";
    return src;
  }

  Image<uint8_t> dst(src.width(), src.height(), format);

  switch (format) {
    case kImageFormatGrayscale:
      convert_grayscale(src, dst);
      break;
    case kImageFormatRGB:
      convert_rgb(src, dst);
      break;
    case kImageFormatRGBA:
      convert_rgba(src, dst);
      break;
    default:
      throw std::invalid_argument("convert()");
      break;
  }

  return dst;
}

} // namespace litehtml
