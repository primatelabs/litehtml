// Copyright (c) 2013, Yuri Kobets (tordex)
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

#ifndef LITEHTML_BORDERS_H__
#define LITEHTML_BORDERS_H__

#include "litehtml/color.h"
#include "litehtml/css/css_length.h"
#include "litehtml/types.h"

namespace litehtml {

struct Border {
    int width = 0;
    BorderStyle style = kBorderStyleNone;
    Color color;

    Border() = default;
};

struct CSSBorder {
    CSSLength width;
    BorderStyle style = kBorderStyleNone;
    Color color;

    CSSBorder() = default;

    Border calculate_border()
    {
        Border border;

        border.width = width.val();
        border.style = style;
        border.color = color;

        return border;
    }
};

struct BorderRadii {
    int top_left_x = 0;
    int top_left_y = 0;

    int top_right_x = 0;
    int top_right_y = 0;

    int bottom_right_x = 0;
    int bottom_right_y = 0;

    int bottom_left_x = 0;
    int bottom_left_y = 0;

    BorderRadii() = default;

    void operator+=(const margins& mg)
    {
        top_left_x += mg.left;
        top_left_y += mg.top;
        top_right_x += mg.right;
        top_right_y += mg.top;
        bottom_right_x += mg.right;
        bottom_right_y += mg.bottom;
        bottom_left_x += mg.left;
        bottom_left_y += mg.bottom;
        fix_values();
    }

    void operator-=(const margins& mg)
    {
        top_left_x -= mg.left;
        top_left_y -= mg.top;
        top_right_x -= mg.right;
        top_right_y -= mg.top;
        bottom_right_x -= mg.right;
        bottom_right_y -= mg.bottom;
        bottom_left_x -= mg.left;
        bottom_left_y -= mg.bottom;
        fix_values();
    }

    void fix_values()
    {
        if (top_left_x < 0)
            top_left_x = 0;
        if (top_left_y < 0)
            top_left_y = 0;
        if (top_right_x < 0)
            top_right_x = 0;
        if (top_right_y < 0)
            top_right_y = 0;
        if (bottom_right_x < 0)
            bottom_right_x = 0;
        if (bottom_right_y < 0)
            bottom_right_y = 0;
        if (bottom_left_x < 0)
            bottom_left_x = 0;
        if (bottom_left_y < 0)
            bottom_left_y = 0;
    }
};

struct CSSBorderRadii {
    CSSLength top_left_x;
    CSSLength top_left_y;

    CSSLength top_right_x;
    CSSLength top_right_y;

    CSSLength bottom_right_x;
    CSSLength bottom_right_y;

    CSSLength bottom_left_x;
    CSSLength bottom_left_y;

    CSSBorderRadii() = default;

    BorderRadii calculate_radii(int width, int height)
    {
        BorderRadii border_radii;

        border_radii.bottom_left_x = bottom_left_x.calc_percent(width);
        border_radii.bottom_left_y = bottom_left_y.calc_percent(height);
        border_radii.top_left_x = top_left_x.calc_percent(width);
        border_radii.top_left_y = top_left_y.calc_percent(height);
        border_radii.top_right_x = top_right_x.calc_percent(width);
        border_radii.top_right_y = top_right_y.calc_percent(height);
        border_radii.bottom_right_x = bottom_right_x.calc_percent(width);
        border_radii.bottom_right_y = bottom_right_y.calc_percent(height);

        return border_radii;
    }
};

struct Borders {
    Border left;
    Border top;
    Border right;
    Border bottom;
    BorderRadii radii;

    Borders() = default;
};

struct CSSBorders {
    CSSBorder left;
    CSSBorder top;
    CSSBorder right;
    CSSBorder bottom;
    CSSBorderRadii radii;

    CSSBorders() = default;

    Borders calculate_borders(int width, int height)
    {
        Borders borders;

        borders.left = left.calculate_border();
        borders.top = top.calculate_border();
        borders.right = right.calculate_border();
        borders.bottom = bottom.calculate_border();
        borders.radii = radii.calculate_radii(width, height);

        return borders;
    }
};

} // namespace litehtml

#endif // LITEHTML_BORDERS_H__
