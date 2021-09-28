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

#ifndef LITEHTML_WEB_COLOR_H__
#define LITEHTML_WEB_COLOR_H__

#include "litehtml/os_types.h"
#include "litehtml/types.h"

namespace litehtml {
struct def_color {
    const tchar_t* name;
    const tchar_t* rgb;
};

extern def_color g_def_colors[];

class document_container;

struct web_color {
    byte blue;
    byte green;
    byte red;
    byte alpha;

    web_color(byte r, byte g, byte b, byte a = 255)
    {
        blue = b;
        green = g;
        red = r;
        alpha = a;
    }

    web_color()
    {
        blue = 0;
        green = 0;
        red = 0;
        alpha = 0xFF;
    }

    web_color(const web_color& val)
    {
        blue = val.blue;
        green = val.green;
        red = val.red;
        alpha = val.alpha;
    }

    web_color& operator=(const web_color& val)
    {
        blue = val.blue;
        green = val.green;
        red = val.red;
        alpha = val.alpha;
        return *this;
    }
    static web_color from_string(const tchar_t* str,
        litehtml::document_container* callback);
    static litehtml::tstring resolve_name(const tchar_t* name,
        litehtml::document_container* callback);
    static bool is_color(const tchar_t* str);
};
} // namespace litehtml

#endif // LITEHTML_WEB_COLOR_H__