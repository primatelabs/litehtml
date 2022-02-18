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

#include "litehtml/color.h"

#include <string.h>

#include <unordered_map>

#include "litehtml/document_container.h"
#include "litehtml/html.h"

namespace litehtml {

namespace {

Color resolve_name(const tstring& name)
{
    static const std::unordered_map<tstring, Color> named_colors = {
        {_t("aliceblue"), Color(0xf0, 0xf8, 0xff)},
        {_t("antiquewhite"), Color(0xfa, 0xeb, 0xd7)},
        {_t("aqua"), Color(0x00, 0xff, 0xff)},
        {_t("aquamarine"), Color(0x7f, 0xff, 0xd4)},
        {_t("azure"), Color(0xf0, 0xff, 0xff)},
        {_t("beige"), Color(0xf5, 0xf5, 0xdc)},
        {_t("bisque"), Color(0xff, 0xe4, 0xc4)},
        {_t("black"), Color(0x00, 0x00, 0x00)},
        {_t("blanchedalmond"), Color(0xff, 0xeb, 0xcd)},
        {_t("blue"), Color(0x00, 0x00, 0xff)},
        {_t("blueviolet"), Color(0x8a, 0x2b, 0xe2)},
        {_t("brown"), Color(0xa5, 0x2a, 0x2a)},
        {_t("burlywood"), Color(0xde, 0xb8, 0x87)},
        {_t("cadetblue"), Color(0x5f, 0x9e, 0xa0)},
        {_t("chartreuse"), Color(0x7f, 0xff, 0x00)},
        {_t("chocolate"), Color(0xd2, 0x69, 0x1e)},
        {_t("coral"), Color(0xff, 0x7f, 0x50)},
        {_t("cornflowerblue"), Color(0x64, 0x95, 0xed)},
        {_t("cornsilk"), Color(0xff, 0xf8, 0xdc)},
        {_t("crimson"), Color(0xdc, 0x14, 0x3c)},
        {_t("cyan"), Color(0x00, 0xff, 0xff)},
        {_t("darkblue"), Color(0x00, 0x00, 0x8b)},
        {_t("darkcyan"), Color(0x00, 0x8b, 0x8b)},
        {_t("darkgoldenrod"), Color(0xb8, 0x86, 0x0b)},
        {_t("darkgray"), Color(0xa9, 0xa9, 0xa9)},
        {_t("darkgrey"), Color(0xa9, 0xa9, 0xa9)},
        {_t("darkgreen"), Color(0x00, 0x64, 0x00)},
        {_t("darkkhaki"), Color(0xbd, 0xb7, 0x6b)},
        {_t("darkmagenta"), Color(0x8b, 0x00, 0x8b)},
        {_t("darkolivegreen"), Color(0x55, 0x6b, 0x2f)},
        {_t("darkorange"), Color(0xff, 0x8c, 0x00)},
        {_t("darkorchid"), Color(0x99, 0x32, 0xcc)},
        {_t("darkred"), Color(0x8b, 0x00, 0x00)},
        {_t("darksalmon"), Color(0xe9, 0x96, 0x7a)},
        {_t("darkseagreen"), Color(0x8f, 0xbc, 0x8f)},
        {_t("darkslateblue"), Color(0x48, 0x3d, 0x8b)},
        {_t("darkslategray"), Color(0x2f, 0x4f, 0x4f)},
        {_t("darkslategrey"), Color(0x2f, 0x4f, 0x4f)},
        {_t("darkturquoise"), Color(0x00, 0xce, 0xd1)},
        {_t("darkviolet"), Color(0x94, 0x00, 0xd3)},
        {_t("deeppink"), Color(0xff, 0x14, 0x93)},
        {_t("deepskyblue"), Color(0x00, 0xbf, 0xff)},
        {_t("dimgray"), Color(0x69, 0x69, 0x69)},
        {_t("dimgrey"), Color(0x69, 0x69, 0x69)},
        {_t("dodgerblue"), Color(0x1e, 0x90, 0xff)},
        {_t("firebrick"), Color(0xb2, 0x22, 0x22)},
        {_t("floralwhite"), Color(0xff, 0xfa, 0xf0)},
        {_t("forestgreen"), Color(0x22, 0x8b, 0x22)},
        {_t("fuchsia"), Color(0xff, 0x00, 0xff)},
        {_t("gainsboro"), Color(0xdc, 0xdc, 0xdc)},
        {_t("ghostwhite"), Color(0xf8, 0xf8, 0xff)},
        {_t("gold"), Color(0xff, 0xd7, 0x00)},
        {_t("goldenrod"), Color(0xda, 0xa5, 0x20)},
        {_t("gray"), Color(0x80, 0x80, 0x80)},
        {_t("grey"), Color(0x80, 0x80, 0x80)},
        {_t("green"), Color(0x00, 0x80, 0x00)},
        {_t("greenyellow"), Color(0xad, 0xff, 0x2f)},
        {_t("honeydew"), Color(0xf0, 0xff, 0xf0)},
        {_t("hotpink"), Color(0xff, 0x69, 0xb4)},
        {_t("ivory"), Color(0xff, 0xff, 0xf0)},
        {_t("khaki"), Color(0xf0, 0xe6, 0x8c)},
        {_t("lavender"), Color(0xe6, 0xe6, 0xfa)},
        {_t("lavenderblush"), Color(0xff, 0xf0, 0xf5)},
        {_t("lawngreen"), Color(0x7c, 0xfc, 0x00)},
        {_t("lemonchiffon"), Color(0xff, 0xfa, 0xcd)},
        {_t("lightblue"), Color(0xad, 0xd8, 0xe6)},
        {_t("lightcoral"), Color(0xf0, 0x80, 0x80)},
        {_t("lightcyan"), Color(0xe0, 0xff, 0xff)},
        {_t("lightgoldenrodyellow"), Color(0xfa, 0xfa, 0xd2)},
        {_t("lightgray"), Color(0xd3, 0xd3, 0xd3)},
        {_t("lightgrey"), Color(0xd3, 0xd3, 0xd3)},
        {_t("lightgreen"), Color(0x90, 0xee, 0x90)},
        {_t("lightpink"), Color(0xff, 0xb6, 0xc1)},
        {_t("lightsalmon"), Color(0xff, 0xa0, 0x7a)},
        {_t("lightseagreen"), Color(0x20, 0xb2, 0xaa)},
        {_t("lightskyblue"), Color(0x87, 0xce, 0xfa)},
        {_t("lightslategray"), Color(0x77, 0x88, 0x99)},
        {_t("lightslategrey"), Color(0x77, 0x88, 0x99)},
        {_t("lightsteelblue"), Color(0xb0, 0xc4, 0xde)},
        {_t("lightyellow"), Color(0xff, 0xff, 0xe0)},
        {_t("lime"), Color(0x00, 0xff, 0x00)},
        {_t("limegreen"), Color(0x32, 0xcd, 0x32)},
        {_t("linen"), Color(0xfa, 0xf0, 0xe6)},
        {_t("magenta"), Color(0xff, 0x00, 0xff)},
        {_t("maroon"), Color(0x80, 0x00, 0x00)},
        {_t("mediumaquamarine"), Color(0x66, 0xcd, 0xaa)},
        {_t("mediumblue"), Color(0x00, 0x00, 0xcd)},
        {_t("mediumorchid"), Color(0xba, 0x55, 0xd3)},
        {_t("mediumpurple"), Color(0x93, 0x70, 0xd8)},
        {_t("mediumseagreen"), Color(0x3c, 0xb3, 0x71)},
        {_t("mediumslateblue"), Color(0x7b, 0x68, 0xee)},
        {_t("mediumspringgreen"), Color(0x00, 0xfa, 0x9a)},
        {_t("mediumturquoise"), Color(0x48, 0xd1, 0xcc)},
        {_t("mediumvioletred"), Color(0xc7, 0x15, 0x85)},
        {_t("midnightblue"), Color(0x19, 0x19, 0x70)},
        {_t("mintcream"), Color(0xf5, 0xff, 0xfa)},
        {_t("mistyrose"), Color(0xff, 0xe4, 0xe1)},
        {_t("moccasin"), Color(0xff, 0xe4, 0xb5)},
        {_t("navajowhite"), Color(0xff, 0xde, 0xad)},
        {_t("navy"), Color(0x00, 0x00, 0x80)},
        {_t("oldlace"), Color(0xfd, 0xf5, 0xe6)},
        {_t("olive"), Color(0x80, 0x80, 0x00)},
        {_t("olivedrab"), Color(0x6b, 0x8e, 0x23)},
        {_t("orange"), Color(0xff, 0xa5, 0x00)},
        {_t("orangered"), Color(0xff, 0x45, 0x00)},
        {_t("orchid"), Color(0xda, 0x70, 0xd6)},
        {_t("palegoldenrod"), Color(0xee, 0xe8, 0xaa)},
        {_t("palegreen"), Color(0x98, 0xfb, 0x98)},
        {_t("paleturquoise"), Color(0xaf, 0xee, 0xee)},
        {_t("palevioletred"), Color(0xd8, 0x70, 0x93)},
        {_t("papayawhip"), Color(0xff, 0xef, 0xd5)},
        {_t("peachpuff"), Color(0xff, 0xda, 0xb9)},
        {_t("peru"), Color(0xcd, 0x85, 0x3f)},
        {_t("pink"), Color(0xff, 0xc0, 0xcb)},
        {_t("plum"), Color(0xdd, 0xa0, 0xdd)},
        {_t("powderblue"), Color(0xb0, 0xe0, 0xe6)},
        {_t("purple"), Color(0x80, 0x00, 0x80)},
        {_t("red"), Color(0xff, 0x00, 0x00)},
        {_t("rosybrown"), Color(0xbc, 0x8f, 0x8f)},
        {_t("royalblue"), Color(0x41, 0x69, 0xe1)},
        {_t("saddlebrown"), Color(0x8b, 0x45, 0x13)},
        {_t("salmon"), Color(0xfa, 0x80, 0x72)},
        {_t("sandybrown"), Color(0xf4, 0xa4, 0x60)},
        {_t("seagreen"), Color(0x2e, 0x8b, 0x57)},
        {_t("seashell"), Color(0xff, 0xf5, 0xee)},
        {_t("sienna"), Color(0xa0, 0x52, 0x2d)},
        {_t("silver"), Color(0xc0, 0xc0, 0xc0)},
        {_t("skyblue"), Color(0x87, 0xce, 0xeb)},
        {_t("slateblue"), Color(0x6a, 0x5a, 0xcd)},
        {_t("slategray"), Color(0x70, 0x80, 0x90)},
        {_t("slategrey"), Color(0x70, 0x80, 0x90)},
        {_t("snow"), Color(0xff, 0xfa, 0xfa)},
        {_t("springgreen"), Color(0x00, 0xff, 0x7f)},
        {_t("steelblue"), Color(0x46, 0x82, 0xb4)},
        {_t("tan"), Color(0xd2, 0xb4, 0x8c)},
        {_t("teal"), Color(0x00, 0x80, 0x80)},
        {_t("thistle"), Color(0xd8, 0xbf, 0xd8)},
        {_t("tomato"), Color(0xff, 0x63, 0x47)},
        {_t("turquoise"), Color(0x40, 0xe0, 0xd0)},
        {_t("violet"), Color(0xee, 0x82, 0xee)},
        {_t("wheat"), Color(0xf5, 0xde, 0xb3)},
        {_t("white"), Color(0xff, 0xff, 0xff)},
        {_t("whitesmoke"), Color(0xf5, 0xf5, 0xf5)},
        {_t("yellow"), Color(0xff, 0xff, 0x00)},
        {_t("yellowgreen"), Color(0x9a, 0xcd, 0x32)},

        {_t("transparent"), Color(0, 0, 0, 0)},
    };

    auto color = named_colors.find(name);
    if (color != named_colors.end()) {
        return color->second;
    }

    return Color();
}

} // namespace

Color::Color(const tstring& str)
{
    *this = from_string(str.c_str());
}

Color Color::from_string(const tchar_t* str)
{
    if (!str || !str[0]) {
        return Color(0, 0, 0);
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
        Color clr;
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

        Color clr;

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

bool Color::is_color(const tchar_t* str)
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
