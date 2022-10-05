// Copyright (c) 2013, Yuri Kobets (tordex)
// Copyright (C) 2020-2022 Primate Labs Inc.
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

#include "litehtml/borders.h"

namespace litehtml {

BorderRadii CSSBorderRadii::calculate_radii(int width, int height)
{
    BorderRadii r;

    r.top_left = top_left.calculate_radius(width, height);
    r.top_right = top_right.calculate_radius(width, height);
    r.bottom_left = bottom_left.calculate_radius(width, height);
    r.bottom_right = bottom_right.calculate_radius(width, height);

    // Handle overlapping corner curves using the algorithm presented in
    // the CSS Backgrounds and Borders Module Level 3 draft:
    // https://www.w3.org/TR/css-backgrounds-3/#corner-overlap
    //
    // calculate_radii() computes the inverse of f to avoid dividing values by
    // zero (which can happen if the radii for an edge are both zero).

    float f = 1.0f;

    if (width > 0) {
      float reciprocal = 1.0f / width;
      f = std::max(f, (r.top_left.x + r.top_right.x) * reciprocal);
      f = std::max(f, (r.bottom_left.x + r.bottom_right.x) * reciprocal);
    }

    if (height > 0) {
      float reciprocal = 1.0f / height;
      f = std::max(f, (r.top_left.y + r.top_right.y) * reciprocal);
      f = std::max(f, (r.bottom_left.y + r.bottom_right.y) * reciprocal);
    }

    f = 1.0f / f;

    r.top_left.x *= f;
    r.top_left.y *= f;

    r.top_right.x *= f;
    r.top_right.y *= f;

    r.bottom_left.x *= f;
    r.bottom_left.y *= f;

    r.bottom_right.x *= f;
    r.bottom_right.y *= f;

    return r;
}

} // namespace litehtml
