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

struct css_border {
    CSSLength width;
    BorderStyle style;
    Color color;

    css_border()
    {
        style = kBorderStyleNone;
    }

    css_border(const css_border& val)
    {
        width = val.width;
        style = val.style;
        color = val.color;
    }

    css_border& operator=(const css_border& val)
    {
        width = val.width;
        style = val.style;
        color = val.color;
        return *this;
    }
};

struct border {
    int width;
    BorderStyle style;
    Color color;

    border()
    {
        width = 0;
    }
    border(const border& val)
    {
        width = val.width;
        style = val.style;
        color = val.color;
    }
    border(const css_border& val)
    {
        width = (int)val.width.val();
        style = val.style;
        color = val.color;
    }
    border& operator=(const border& val)
    {
        width = val.width;
        style = val.style;
        color = val.color;
        return *this;
    }
    border& operator=(const css_border& val)
    {
        width = (int)val.width.val();
        style = val.style;
        color = val.color;
        return *this;
    }
};

struct border_radiuses {
    int top_left_x;
    int top_left_y;

    int top_right_x;
    int top_right_y;

    int bottom_right_x;
    int bottom_right_y;

    int bottom_left_x;
    int bottom_left_y;

    border_radiuses()
    {
        top_left_x = 0;
        top_left_y = 0;
        top_right_x = 0;
        top_right_y = 0;
        bottom_right_x = 0;
        bottom_right_y = 0;
        bottom_left_x = 0;
        bottom_left_y = 0;
    }
    border_radiuses(const border_radiuses& val)
    {
        top_left_x = val.top_left_x;
        top_left_y = val.top_left_y;
        top_right_x = val.top_right_x;
        top_right_y = val.top_right_y;
        bottom_right_x = val.bottom_right_x;
        bottom_right_y = val.bottom_right_y;
        bottom_left_x = val.bottom_left_x;
        bottom_left_y = val.bottom_left_y;
    }
    border_radiuses& operator=(const border_radiuses& val)
    {
        top_left_x = val.top_left_x;
        top_left_y = val.top_left_y;
        top_right_x = val.top_right_x;
        top_right_y = val.top_right_y;
        bottom_right_x = val.bottom_right_x;
        bottom_right_y = val.bottom_right_y;
        bottom_left_x = val.bottom_left_x;
        bottom_left_y = val.bottom_left_y;
        return *this;
    }
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

struct css_border_radius {
    CSSLength top_left_x;
    CSSLength top_left_y;

    CSSLength top_right_x;
    CSSLength top_right_y;

    CSSLength bottom_right_x;
    CSSLength bottom_right_y;

    CSSLength bottom_left_x;
    CSSLength bottom_left_y;

    css_border_radius()
    {
    }

    css_border_radius(const css_border_radius& val)
    {
        top_left_x = val.top_left_x;
        top_left_y = val.top_left_y;
        top_right_x = val.top_right_x;
        top_right_y = val.top_right_y;
        bottom_left_x = val.bottom_left_x;
        bottom_left_y = val.bottom_left_y;
        bottom_right_x = val.bottom_right_x;
        bottom_right_y = val.bottom_right_y;
    }

    css_border_radius& operator=(const css_border_radius& val)
    {
        top_left_x = val.top_left_x;
        top_left_y = val.top_left_y;
        top_right_x = val.top_right_x;
        top_right_y = val.top_right_y;
        bottom_left_x = val.bottom_left_x;
        bottom_left_y = val.bottom_left_y;
        bottom_right_x = val.bottom_right_x;
        bottom_right_y = val.bottom_right_y;
        return *this;
    }
    border_radiuses calc_percents(int width, int height)
    {
        border_radiuses ret;
        ret.bottom_left_x = bottom_left_x.calc_percent(width);
        ret.bottom_left_y = bottom_left_y.calc_percent(height);
        ret.top_left_x = top_left_x.calc_percent(width);
        ret.top_left_y = top_left_y.calc_percent(height);
        ret.top_right_x = top_right_x.calc_percent(width);
        ret.top_right_y = top_right_y.calc_percent(height);
        ret.bottom_right_x = bottom_right_x.calc_percent(width);
        ret.bottom_right_y = bottom_right_y.calc_percent(height);
        return ret;
    }
};

struct css_borders {
    css_border left;
    css_border top;
    css_border right;
    css_border bottom;
    css_border_radius radius;

    css_borders()
    {
    }

    css_borders(const css_borders& val)
    {
        left = val.left;
        right = val.right;
        top = val.top;
        bottom = val.bottom;
        radius = val.radius;
    }

    css_borders& operator=(const css_borders& val)
    {
        left = val.left;
        right = val.right;
        top = val.top;
        bottom = val.bottom;
        radius = val.radius;
        return *this;
    }
};

struct borders {
    border left;
    border top;
    border right;
    border bottom;
    border_radiuses radius;

    borders()
    {
    }

    borders(const borders& val)
    {
        left = val.left;
        right = val.right;
        top = val.top;
        bottom = val.bottom;
        radius = val.radius;
    }

    borders(const css_borders& val)
    {
        left = val.left;
        right = val.right;
        top = val.top;
        bottom = val.bottom;
    }

    borders& operator=(const borders& val)
    {
        left = val.left;
        right = val.right;
        top = val.top;
        bottom = val.bottom;
        radius = val.radius;
        return *this;
    }

    borders& operator=(const css_borders& val)
    {
        left = val.left;
        right = val.right;
        top = val.top;
        bottom = val.bottom;
        return *this;
    }
};
} // namespace litehtml

#endif // LITEHTML_BORDERS_H__