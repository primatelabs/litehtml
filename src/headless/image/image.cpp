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

#include "image/image.h"

#include <string.h>

#include <algorithm>
#include <fstream>
#include <iostream>

#include "image/jpeg_codec.h"
#include "image/png_codec.h"

namespace headless {

// Returns the number of channels for the given image format.
int format_channels(ImageFormat format)
{
  switch (format) {
    case kImageFormatGrayscale:
      return 1;
    case kImageFormatGrayscaleAlpha:
      return 2;
    case kImageFormatRGB:
      return 3;
    case kImageFormatRGBA:
      return 4;
    default:
      return 0;
  }
}

// Returns the image format for the given number of channels.  This function
// will become problematic once there are multiple image formats with the same
// number of channels (e.g., if we add support for BGR images).
ImageFormat channels_format(int channels)
{
  switch (channels) {
    case 1:
      return kImageFormatGrayscale;
    case 2:
      return kImageFormatGrayscaleAlpha;
    case 3:
      return kImageFormatRGB;
    case 4:
      return kImageFormatRGBA;
    default:
      return kImageFormatNone;
  }
}

template<>
uint8_t Image<uint8_t>::maximum() const
{
  return 255;
}

template<>
uint16_t Image<uint16_t>::maximum() const
{
  return 65535;
}

template <>
float Image<float>::maximum() const
{
  return 1.0f;
}

} // namespace headless
