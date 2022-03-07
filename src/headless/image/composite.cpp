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

#include "image/composite.h"

#include <assert.h>

#include "litehtml/logging.h"

namespace headless {

void composite(Image<uint8_t>& u, const Image<uint8_t>& v)
{
  // The other image must have an alpha channel.
  assert(v.format() == kImageFormatRGBA);

  const int width = std::min(u.width(), v.width());
  const int height = std::min(u.height(), v.height());
  const int channels = u.channels();
  const int other_channels = v.channels();

  for (int y = 0; y < height; y++) {
    const uint8_t* other_row = v.row(y);
    uint8_t* this_row = u.row(y);

    for (int x = 0; x < width; x++) {
      unsigned int alpha = other_row[3];
      unsigned int inverse_alpha = 255 - alpha;
      for (int c = 0; c < std::min(channels, 3); c++) {
        unsigned int tmp = (other_row[c] * alpha + this_row[c] * inverse_alpha) / 255;
        this_row[c] = tmp;
      }

      this_row += channels;
      other_row += other_channels;
    }
  }
}

} // namespace headless
