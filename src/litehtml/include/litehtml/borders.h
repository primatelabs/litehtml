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

#ifndef LITEHTML_BORDERS_H__
#define LITEHTML_BORDERS_H__

#include <algorithm>

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

struct BorderRadius {
    int x = 0;
    int y = 0;

    BorderRadius() = default;

    void fix_values()
    {
        if (x < 0) {
            x = 0;
        }
        if (y < 0) {
            y = 0;
        }
    }

    bool has_radius() const
    {
        return (x > 0) && (y > 0);
    }
};

struct BorderRadii {
    BorderRadius top_left;
    BorderRadius top_right;
    BorderRadius bottom_left;
    BorderRadius bottom_right;

    BorderRadii() = default;

    void operator+=(const Margins& mg)
    {
        top_left.x += mg.left;
        top_left.y += mg.top;
        top_right.x += mg.right;
        top_right.y += mg.top;
        bottom_left.x += mg.left;
        bottom_left.y += mg.bottom;
        bottom_right.x += mg.right;
        bottom_right.y += mg.bottom;
        fix_values();
    }

    void operator-=(const Margins& mg)
    {
        top_left.x -= mg.left;
        top_left.y -= mg.top;
        top_right.x -= mg.right;
        top_right.y -= mg.top;
        bottom_left.x -= mg.left;
        bottom_left.y -= mg.bottom;
        bottom_right.x -= mg.right;
        bottom_right.y -= mg.bottom;
        fix_values();
    }

    void fix_values()
    {
        top_left.fix_values();
        top_right.fix_values();
        bottom_left.fix_values();
        bottom_right.fix_values();
    }

    bool has_radius() const
    {
        return top_left.has_radius() || top_right.has_radius() || bottom_left.has_radius() || bottom_right.has_radius();
    }
};

struct CSSBorderRadius {
    CSSLength x;
    CSSLength y;

    BorderRadius calculate_radius(int width, int height)
    {
        BorderRadius border_radius;

        border_radius.x = x.calc_percent(width);
        border_radius.y = x.calc_percent(height);

        return border_radius;
    }
};

struct CSSBorderRadii {
    CSSBorderRadius top_left;
    CSSBorderRadius top_right;
    CSSBorderRadius bottom_left;
    CSSBorderRadius bottom_right;

    CSSBorderRadii() = default;

    BorderRadii calculate_radii(int width, int height);
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
