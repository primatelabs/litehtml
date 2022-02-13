// Copyright (c) 2013, Yuri Kobets (tordex)
// Copyright (c) 2020-2021 Primate Labs Inc.
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

#include "litehtml/web_color.h"

#include <string.h>

#include <unordered_map>

#include "litehtml/document_container.h"
#include "litehtml/html.h"

namespace litehtml {

namespace {

WebColor resolve_name(const tstring& name)
{
    static std::unordered_map<tstring, WebColor> named_colors = {
        {_t("aliceblue"), WebColor(0xf0, 0xf8, 0xff)},
        {_t("antiquewhite"), WebColor(0xfa, 0xeb, 0xd7)},
        {_t("aqua"), WebColor(0x00, 0xff, 0xff)},
        {_t("aquamarine"), WebColor(0x7f, 0xff, 0xd4)},
        {_t("azure"), WebColor(0xf0, 0xff, 0xff)},
        {_t("beige"), WebColor(0xf5, 0xf5, 0xdc)},
        {_t("bisque"), WebColor(0xff, 0xe4, 0xc4)},
        {_t("black"), WebColor(0x00, 0x00, 0x00)},
        {_t("blanchedalmond"), WebColor(0xff, 0xeb, 0xcd)},
        {_t("blue"), WebColor(0x00, 0x00, 0xff)},
        {_t("blueviolet"), WebColor(0x8a, 0x2b, 0xe2)},
        {_t("brown"), WebColor(0xa5, 0x2a, 0x2a)},
        {_t("burlywood"), WebColor(0xde, 0xb8, 0x87)},
        {_t("cadetblue"), WebColor(0x5f, 0x9e, 0xa0)},
        {_t("chartreuse"), WebColor(0x7f, 0xff, 0x00)},
        {_t("chocolate"), WebColor(0xd2, 0x69, 0x1e)},
        {_t("coral"), WebColor(0xff, 0x7f, 0x50)},
        {_t("cornflowerblue"), WebColor(0x64, 0x95, 0xed)},
        {_t("cornsilk"), WebColor(0xff, 0xf8, 0xdc)},
        {_t("crimson"), WebColor(0xdc, 0x14, 0x3c)},
        {_t("cyan"), WebColor(0x00, 0xff, 0xff)},
        {_t("darkblue"), WebColor(0x00, 0x00, 0x8b)},
        {_t("darkcyan"), WebColor(0x00, 0x8b, 0x8b)},
        {_t("darkgoldenrod"), WebColor(0xb8, 0x86, 0x0b)},
        {_t("darkgray"), WebColor(0xa9, 0xa9, 0xa9)},
        {_t("darkgrey"), WebColor(0xa9, 0xa9, 0xa9)},
        {_t("darkgreen"), WebColor(0x00, 0x64, 0x00)},
        {_t("darkkhaki"), WebColor(0xbd, 0xb7, 0x6b)},
        {_t("darkmagenta"), WebColor(0x8b, 0x00, 0x8b)},
        {_t("darkolivegreen"), WebColor(0x55, 0x6b, 0x2f)},
        {_t("darkorange"), WebColor(0xff, 0x8c, 0x00)},
        {_t("darkorchid"), WebColor(0x99, 0x32, 0xcc)},
        {_t("darkred"), WebColor(0x8b, 0x00, 0x00)},
        {_t("darksalmon"), WebColor(0xe9, 0x96, 0x7a)},
        {_t("darkseagreen"), WebColor(0x8f, 0xbc, 0x8f)},
        {_t("darkslateblue"), WebColor(0x48, 0x3d, 0x8b)},
        {_t("darkslategray"), WebColor(0x2f, 0x4f, 0x4f)},
        {_t("darkslategrey"), WebColor(0x2f, 0x4f, 0x4f)},
        {_t("darkturquoise"), WebColor(0x00, 0xce, 0xd1)},
        {_t("darkviolet"), WebColor(0x94, 0x00, 0xd3)},
        {_t("deeppink"), WebColor(0xff, 0x14, 0x93)},
        {_t("deepskyblue"), WebColor(0x00, 0xbf, 0xff)},
        {_t("dimgray"), WebColor(0x69, 0x69, 0x69)},
        {_t("dimgrey"), WebColor(0x69, 0x69, 0x69)},
        {_t("dodgerblue"), WebColor(0x1e, 0x90, 0xff)},
        {_t("firebrick"), WebColor(0xb2, 0x22, 0x22)},
        {_t("floralwhite"), WebColor(0xff, 0xfa, 0xf0)},
        {_t("forestgreen"), WebColor(0x22, 0x8b, 0x22)},
        {_t("fuchsia"), WebColor(0xff, 0x00, 0xff)},
        {_t("gainsboro"), WebColor(0xdc, 0xdc, 0xdc)},
        {_t("ghostwhite"), WebColor(0xf8, 0xf8, 0xff)},
        {_t("gold"), WebColor(0xff, 0xd7, 0x00)},
        {_t("goldenrod"), WebColor(0xda, 0xa5, 0x20)},
        {_t("gray"), WebColor(0x80, 0x80, 0x80)},
        {_t("grey"), WebColor(0x80, 0x80, 0x80)},
        {_t("green"), WebColor(0x00, 0x80, 0x00)},
        {_t("greenyellow"), WebColor(0xad, 0xff, 0x2f)},
        {_t("honeydew"), WebColor(0xf0, 0xff, 0xf0)},
        {_t("hotpink"), WebColor(0xff, 0x69, 0xb4)},
        {_t("ivory"), WebColor(0xff, 0xff, 0xf0)},
        {_t("khaki"), WebColor(0xf0, 0xe6, 0x8c)},
        {_t("lavender"), WebColor(0xe6, 0xe6, 0xfa)},
        {_t("lavenderblush"), WebColor(0xff, 0xf0, 0xf5)},
        {_t("lawngreen"), WebColor(0x7c, 0xfc, 0x00)},
        {_t("lemonchiffon"), WebColor(0xff, 0xfa, 0xcd)},
        {_t("lightblue"), WebColor(0xad, 0xd8, 0xe6)},
        {_t("lightcoral"), WebColor(0xf0, 0x80, 0x80)},
        {_t("lightcyan"), WebColor(0xe0, 0xff, 0xff)},
        {_t("lightgoldenrodyellow"), WebColor(0xfa, 0xfa, 0xd2)},
        {_t("lightgray"), WebColor(0xd3, 0xd3, 0xd3)},
        {_t("lightgrey"), WebColor(0xd3, 0xd3, 0xd3)},
        {_t("lightgreen"), WebColor(0x90, 0xee, 0x90)},
        {_t("lightpink"), WebColor(0xff, 0xb6, 0xc1)},
        {_t("lightsalmon"), WebColor(0xff, 0xa0, 0x7a)},
        {_t("lightseagreen"), WebColor(0x20, 0xb2, 0xaa)},
        {_t("lightskyblue"), WebColor(0x87, 0xce, 0xfa)},
        {_t("lightslategray"), WebColor(0x77, 0x88, 0x99)},
        {_t("lightslategrey"), WebColor(0x77, 0x88, 0x99)},
        {_t("lightsteelblue"), WebColor(0xb0, 0xc4, 0xde)},
        {_t("lightyellow"), WebColor(0xff, 0xff, 0xe0)},
        {_t("lime"), WebColor(0x00, 0xff, 0x00)},
        {_t("limegreen"), WebColor(0x32, 0xcd, 0x32)},
        {_t("linen"), WebColor(0xfa, 0xf0, 0xe6)},
        {_t("magenta"), WebColor(0xff, 0x00, 0xff)},
        {_t("maroon"), WebColor(0x80, 0x00, 0x00)},
        {_t("mediumaquamarine"), WebColor(0x66, 0xcd, 0xaa)},
        {_t("mediumblue"), WebColor(0x00, 0x00, 0xcd)},
        {_t("mediumorchid"), WebColor(0xba, 0x55, 0xd3)},
        {_t("mediumpurple"), WebColor(0x93, 0x70, 0xd8)},
        {_t("mediumseagreen"), WebColor(0x3c, 0xb3, 0x71)},
        {_t("mediumslateblue"), WebColor(0x7b, 0x68, 0xee)},
        {_t("mediumspringgreen"), WebColor(0x00, 0xfa, 0x9a)},
        {_t("mediumturquoise"), WebColor(0x48, 0xd1, 0xcc)},
        {_t("mediumvioletred"), WebColor(0xc7, 0x15, 0x85)},
        {_t("midnightblue"), WebColor(0x19, 0x19, 0x70)},
        {_t("mintcream"), WebColor(0xf5, 0xff, 0xfa)},
        {_t("mistyrose"), WebColor(0xff, 0xe4, 0xe1)},
        {_t("moccasin"), WebColor(0xff, 0xe4, 0xb5)},
        {_t("navajowhite"), WebColor(0xff, 0xde, 0xad)},
        {_t("navy"), WebColor(0x00, 0x00, 0x80)},
        {_t("oldlace"), WebColor(0xfd, 0xf5, 0xe6)},
        {_t("olive"), WebColor(0x80, 0x80, 0x00)},
        {_t("olivedrab"), WebColor(0x6b, 0x8e, 0x23)},
        {_t("orange"), WebColor(0xff, 0xa5, 0x00)},
        {_t("orangered"), WebColor(0xff, 0x45, 0x00)},
        {_t("orchid"), WebColor(0xda, 0x70, 0xd6)},
        {_t("palegoldenrod"), WebColor(0xee, 0xe8, 0xaa)},
        {_t("palegreen"), WebColor(0x98, 0xfb, 0x98)},
        {_t("paleturquoise"), WebColor(0xaf, 0xee, 0xee)},
        {_t("palevioletred"), WebColor(0xd8, 0x70, 0x93)},
        {_t("papayawhip"), WebColor(0xff, 0xef, 0xd5)},
        {_t("peachpuff"), WebColor(0xff, 0xda, 0xb9)},
        {_t("peru"), WebColor(0xcd, 0x85, 0x3f)},
        {_t("pink"), WebColor(0xff, 0xc0, 0xcb)},
        {_t("plum"), WebColor(0xdd, 0xa0, 0xdd)},
        {_t("powderblue"), WebColor(0xb0, 0xe0, 0xe6)},
        {_t("purple"), WebColor(0x80, 0x00, 0x80)},
        {_t("red"), WebColor(0xff, 0x00, 0x00)},
        {_t("rosybrown"), WebColor(0xbc, 0x8f, 0x8f)},
        {_t("royalblue"), WebColor(0x41, 0x69, 0xe1)},
        {_t("saddlebrown"), WebColor(0x8b, 0x45, 0x13)},
        {_t("salmon"), WebColor(0xfa, 0x80, 0x72)},
        {_t("sandybrown"), WebColor(0xf4, 0xa4, 0x60)},
        {_t("seagreen"), WebColor(0x2e, 0x8b, 0x57)},
        {_t("seashell"), WebColor(0xff, 0xf5, 0xee)},
        {_t("sienna"), WebColor(0xa0, 0x52, 0x2d)},
        {_t("silver"), WebColor(0xc0, 0xc0, 0xc0)},
        {_t("skyblue"), WebColor(0x87, 0xce, 0xeb)},
        {_t("slateblue"), WebColor(0x6a, 0x5a, 0xcd)},
        {_t("slategray"), WebColor(0x70, 0x80, 0x90)},
        {_t("slategrey"), WebColor(0x70, 0x80, 0x90)},
        {_t("snow"), WebColor(0xff, 0xfa, 0xfa)},
        {_t("springgreen"), WebColor(0x00, 0xff, 0x7f)},
        {_t("steelblue"), WebColor(0x46, 0x82, 0xb4)},
        {_t("tan"), WebColor(0xd2, 0xb4, 0x8c)},
        {_t("teal"), WebColor(0x00, 0x80, 0x80)},
        {_t("thistle"), WebColor(0xd8, 0xbf, 0xd8)},
        {_t("tomato"), WebColor(0xff, 0x63, 0x47)},
        {_t("turquoise"), WebColor(0x40, 0xe0, 0xd0)},
        {_t("violet"), WebColor(0xee, 0x82, 0xee)},
        {_t("wheat"), WebColor(0xf5, 0xde, 0xb3)},
        {_t("white"), WebColor(0xff, 0xff, 0xff)},
        {_t("whitesmoke"), WebColor(0xf5, 0xf5, 0xf5)},
        {_t("yellow"), WebColor(0xff, 0xff, 0x00)},
        {_t("yellowgreen"), WebColor(0x9a, 0xcd, 0x32)},

        {_t("transparent"), WebColor(0, 0, 0, 0)},
    };

    return named_colors[name];
}

} // namespace

WebColor::WebColor(const tstring& str)
{
    *this = from_string(str.c_str());
}

WebColor WebColor::from_string(const tchar_t* str)
{
    if (!str || !str[0]) {
        return WebColor(0, 0, 0);
    }

    if (str[0] == _t('#')) {
        tstring red = _t("");
        tstring green = _t("");
        tstring blue = _t("");
        if (t_strlen(str + 1) == 3) {
            red += str[1];
            red += str[1];
            green += str[2];
            green += str[2];
            blue += str[3];
            blue += str[3];
        } else if (t_strlen(str + 1) == 6) {
            red += str[1];
            red += str[2];
            green += str[3];
            green += str[4];
            blue += str[5];
            blue += str[6];
        }
        tchar_t* sss = nullptr;
        WebColor clr;
        clr.red = (byte)t_strtol(red.c_str(), &sss, 16);
        clr.green = (byte)t_strtol(green.c_str(), &sss, 16);
        clr.blue = (byte)t_strtol(blue.c_str(), &sss, 16);
        return clr;
    } else if (!t_strncmp(str, _t("rgb"), 3)) {
        tstring s = str;

        tstring::size_type pos = s.find_first_of(_t("("));
        if (pos != tstring::npos) {
            s.erase(s.begin(), s.begin() + pos + 1);
        }
        pos = s.find_last_of(_t(")"));
        if (pos != tstring::npos) {
            s.erase(s.begin() + pos, s.end());
        }

        std::vector<tstring> tokens;
        split_string(s, tokens, _t(", \t"));

        WebColor clr;

        if (tokens.size() >= 1)
            clr.red = (byte)t_atoi(tokens[0].c_str());
        if (tokens.size() >= 2)
            clr.green = (byte)t_atoi(tokens[1].c_str());
        if (tokens.size() >= 3)
            clr.blue = (byte)t_atoi(tokens[2].c_str());
        if (tokens.size() >= 4)
            clr.alpha = (byte)(t_strtod(tokens[3].c_str(), nullptr) * 255.0);

        return clr;
    } else {
        return resolve_name(str);
    }
}

bool WebColor::is_color(const tchar_t* str)
{
    if (!t_strncasecmp(str, _t("rgb"), 3) || str[0] == _t('#')) {
        return true;
    }
    if (!t_isdigit(str[0]) && str[0] != _t('.')) {
        return true;
    }

    return false;
}

} // namespace litehtml
