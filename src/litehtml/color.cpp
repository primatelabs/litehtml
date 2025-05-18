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

Color resolve_name(const std::string& name)
{
    static const std::unordered_map<std::string, Color> named_colors = {
        {"aliceblue", Color(0xf0, 0xf8, 0xff)},
        {"antiquewhite", Color(0xfa, 0xeb, 0xd7)},
        {"aqua", Color(0x00, 0xff, 0xff)},
        {"aquamarine", Color(0x7f, 0xff, 0xd4)},
        {"azure", Color(0xf0, 0xff, 0xff)},
        {"beige", Color(0xf5, 0xf5, 0xdc)},
        {"bisque", Color(0xff, 0xe4, 0xc4)},
        {"black", Color(0x00, 0x00, 0x00)},
        {"blanchedalmond", Color(0xff, 0xeb, 0xcd)},
        {"blue", Color(0x00, 0x00, 0xff)},
        {"blueviolet", Color(0x8a, 0x2b, 0xe2)},
        {"brown", Color(0xa5, 0x2a, 0x2a)},
        {"burlywood", Color(0xde, 0xb8, 0x87)},
        {"cadetblue", Color(0x5f, 0x9e, 0xa0)},
        {"chartreuse", Color(0x7f, 0xff, 0x00)},
        {"chocolate", Color(0xd2, 0x69, 0x1e)},
        {"coral", Color(0xff, 0x7f, 0x50)},
        {"cornflowerblue", Color(0x64, 0x95, 0xed)},
        {"cornsilk", Color(0xff, 0xf8, 0xdc)},
        {"crimson", Color(0xdc, 0x14, 0x3c)},
        {"cyan", Color(0x00, 0xff, 0xff)},
        {"darkblue", Color(0x00, 0x00, 0x8b)},
        {"darkcyan", Color(0x00, 0x8b, 0x8b)},
        {"darkgoldenrod", Color(0xb8, 0x86, 0x0b)},
        {"darkgray", Color(0xa9, 0xa9, 0xa9)},
        {"darkgrey", Color(0xa9, 0xa9, 0xa9)},
        {"darkgreen", Color(0x00, 0x64, 0x00)},
        {"darkkhaki", Color(0xbd, 0xb7, 0x6b)},
        {"darkmagenta", Color(0x8b, 0x00, 0x8b)},
        {"darkolivegreen", Color(0x55, 0x6b, 0x2f)},
        {"darkorange", Color(0xff, 0x8c, 0x00)},
        {"darkorchid", Color(0x99, 0x32, 0xcc)},
        {"darkred", Color(0x8b, 0x00, 0x00)},
        {"darksalmon", Color(0xe9, 0x96, 0x7a)},
        {"darkseagreen", Color(0x8f, 0xbc, 0x8f)},
        {"darkslateblue", Color(0x48, 0x3d, 0x8b)},
        {"darkslategray", Color(0x2f, 0x4f, 0x4f)},
        {"darkslategrey", Color(0x2f, 0x4f, 0x4f)},
        {"darkturquoise", Color(0x00, 0xce, 0xd1)},
        {"darkviolet", Color(0x94, 0x00, 0xd3)},
        {"deeppink", Color(0xff, 0x14, 0x93)},
        {"deepskyblue", Color(0x00, 0xbf, 0xff)},
        {"dimgray", Color(0x69, 0x69, 0x69)},
        {"dimgrey", Color(0x69, 0x69, 0x69)},
        {"dodgerblue", Color(0x1e, 0x90, 0xff)},
        {"firebrick", Color(0xb2, 0x22, 0x22)},
        {"floralwhite", Color(0xff, 0xfa, 0xf0)},
        {"forestgreen", Color(0x22, 0x8b, 0x22)},
        {"fuchsia", Color(0xff, 0x00, 0xff)},
        {"gainsboro", Color(0xdc, 0xdc, 0xdc)},
        {"ghostwhite", Color(0xf8, 0xf8, 0xff)},
        {"gold", Color(0xff, 0xd7, 0x00)},
        {"goldenrod", Color(0xda, 0xa5, 0x20)},
        {"gray", Color(0x80, 0x80, 0x80)},
        {"grey", Color(0x80, 0x80, 0x80)},
        {"green", Color(0x00, 0x80, 0x00)},
        {"greenyellow", Color(0xad, 0xff, 0x2f)},
        {"honeydew", Color(0xf0, 0xff, 0xf0)},
        {"hotpink", Color(0xff, 0x69, 0xb4)},
        {"ivory", Color(0xff, 0xff, 0xf0)},
        {"khaki", Color(0xf0, 0xe6, 0x8c)},
        {"lavender", Color(0xe6, 0xe6, 0xfa)},
        {"lavenderblush", Color(0xff, 0xf0, 0xf5)},
        {"lawngreen", Color(0x7c, 0xfc, 0x00)},
        {"lemonchiffon", Color(0xff, 0xfa, 0xcd)},
        {"lightblue", Color(0xad, 0xd8, 0xe6)},
        {"lightcoral", Color(0xf0, 0x80, 0x80)},
        {"lightcyan", Color(0xe0, 0xff, 0xff)},
        {"lightgoldenrodyellow", Color(0xfa, 0xfa, 0xd2)},
        {"lightgray", Color(0xd3, 0xd3, 0xd3)},
        {"lightgrey", Color(0xd3, 0xd3, 0xd3)},
        {"lightgreen", Color(0x90, 0xee, 0x90)},
        {"lightpink", Color(0xff, 0xb6, 0xc1)},
        {"lightsalmon", Color(0xff, 0xa0, 0x7a)},
        {"lightseagreen", Color(0x20, 0xb2, 0xaa)},
        {"lightskyblue", Color(0x87, 0xce, 0xfa)},
        {"lightslategray", Color(0x77, 0x88, 0x99)},
        {"lightslategrey", Color(0x77, 0x88, 0x99)},
        {"lightsteelblue", Color(0xb0, 0xc4, 0xde)},
        {"lightyellow", Color(0xff, 0xff, 0xe0)},
        {"lime", Color(0x00, 0xff, 0x00)},
        {"limegreen", Color(0x32, 0xcd, 0x32)},
        {"linen", Color(0xfa, 0xf0, 0xe6)},
        {"magenta", Color(0xff, 0x00, 0xff)},
        {"maroon", Color(0x80, 0x00, 0x00)},
        {"mediumaquamarine", Color(0x66, 0xcd, 0xaa)},
        {"mediumblue", Color(0x00, 0x00, 0xcd)},
        {"mediumorchid", Color(0xba, 0x55, 0xd3)},
        {"mediumpurple", Color(0x93, 0x70, 0xd8)},
        {"mediumseagreen", Color(0x3c, 0xb3, 0x71)},
        {"mediumslateblue", Color(0x7b, 0x68, 0xee)},
        {"mediumspringgreen", Color(0x00, 0xfa, 0x9a)},
        {"mediumturquoise", Color(0x48, 0xd1, 0xcc)},
        {"mediumvioletred", Color(0xc7, 0x15, 0x85)},
        {"midnightblue", Color(0x19, 0x19, 0x70)},
        {"mintcream", Color(0xf5, 0xff, 0xfa)},
        {"mistyrose", Color(0xff, 0xe4, 0xe1)},
        {"moccasin", Color(0xff, 0xe4, 0xb5)},
        {"navajowhite", Color(0xff, 0xde, 0xad)},
        {"navy", Color(0x00, 0x00, 0x80)},
        {"oldlace", Color(0xfd, 0xf5, 0xe6)},
        {"olive", Color(0x80, 0x80, 0x00)},
        {"olivedrab", Color(0x6b, 0x8e, 0x23)},
        {"orange", Color(0xff, 0xa5, 0x00)},
        {"orangered", Color(0xff, 0x45, 0x00)},
        {"orchid", Color(0xda, 0x70, 0xd6)},
        {"palegoldenrod", Color(0xee, 0xe8, 0xaa)},
        {"palegreen", Color(0x98, 0xfb, 0x98)},
        {"paleturquoise", Color(0xaf, 0xee, 0xee)},
        {"palevioletred", Color(0xd8, 0x70, 0x93)},
        {"papayawhip", Color(0xff, 0xef, 0xd5)},
        {"peachpuff", Color(0xff, 0xda, 0xb9)},
        {"peru", Color(0xcd, 0x85, 0x3f)},
        {"pink", Color(0xff, 0xc0, 0xcb)},
        {"plum", Color(0xdd, 0xa0, 0xdd)},
        {"powderblue", Color(0xb0, 0xe0, 0xe6)},
        {"purple", Color(0x80, 0x00, 0x80)},
        {"red", Color(0xff, 0x00, 0x00)},
        {"rosybrown", Color(0xbc, 0x8f, 0x8f)},
        {"royalblue", Color(0x41, 0x69, 0xe1)},
        {"saddlebrown", Color(0x8b, 0x45, 0x13)},
        {"salmon", Color(0xfa, 0x80, 0x72)},
        {"sandybrown", Color(0xf4, 0xa4, 0x60)},
        {"seagreen", Color(0x2e, 0x8b, 0x57)},
        {"seashell", Color(0xff, 0xf5, 0xee)},
        {"sienna", Color(0xa0, 0x52, 0x2d)},
        {"silver", Color(0xc0, 0xc0, 0xc0)},
        {"skyblue", Color(0x87, 0xce, 0xeb)},
        {"slateblue", Color(0x6a, 0x5a, 0xcd)},
        {"slategray", Color(0x70, 0x80, 0x90)},
        {"slategrey", Color(0x70, 0x80, 0x90)},
        {"snow", Color(0xff, 0xfa, 0xfa)},
        {"springgreen", Color(0x00, 0xff, 0x7f)},
        {"steelblue", Color(0x46, 0x82, 0xb4)},
        {"tan", Color(0xd2, 0xb4, 0x8c)},
        {"teal", Color(0x00, 0x80, 0x80)},
        {"thistle", Color(0xd8, 0xbf, 0xd8)},
        {"tomato", Color(0xff, 0x63, 0x47)},
        {"turquoise", Color(0x40, 0xe0, 0xd0)},
        {"violet", Color(0xee, 0x82, 0xee)},
        {"wheat", Color(0xf5, 0xde, 0xb3)},
        {"white", Color(0xff, 0xff, 0xff)},
        {"whitesmoke", Color(0xf5, 0xf5, 0xf5)},
        {"yellow", Color(0xff, 0xff, 0x00)},
        {"yellowgreen", Color(0x9a, 0xcd, 0x32)},

        {"transparent", Color(0, 0, 0, 0)},
    };

    auto color = named_colors.find(name);
    if (color != named_colors.end()) {
        return color->second;
    }

    return Color();
}

} // namespace

Color::Color(const std::string& str)
{
    *this = from_string(str.c_str());
}

Color Color::from_string(const char* str)
{
    if (!str || !str[0]) {
        return Color(0, 0, 0);
    }

    if (str[0] == '#') {
        std::string red = "";
        std::string green = "";
        std::string blue = "";
        if (strlen(str + 1) == 3) {
            red += str[1];
            red += str[1];
            green += str[2];
            green += str[2];
            blue += str[3];
            blue += str[3];
        } else if (strlen(str + 1) == 6) {
            red += str[1];
            red += str[2];
            green += str[3];
            green += str[4];
            blue += str[5];
            blue += str[6];
        }
        char* sss = nullptr;
        Color clr;
        clr.red = (byte)strtol(red.c_str(), &sss, 16);
        clr.green = (byte)strtol(green.c_str(), &sss, 16);
        clr.blue = (byte)strtol(blue.c_str(), &sss, 16);
        return clr;
    } else if (!strncmp(str, "rgb", 3)) {
        std::string s = str;

        std::string::size_type pos = s.find_first_of("(");
        if (pos != std::string::npos) {
            s.erase(s.begin(), s.begin() + pos + 1);
        }
        pos = s.find_last_of(")");
        if (pos != std::string::npos) {
            s.erase(s.begin() + pos, s.end());
        }

        std::vector<std::string> tokens;
        split_string(s, tokens, ", \t");

        Color clr;

        if (tokens.size() >= 1)
            clr.red = (byte)atoi(tokens[0].c_str());
        if (tokens.size() >= 2)
            clr.green = (byte)atoi(tokens[1].c_str());
        if (tokens.size() >= 3)
            clr.blue = (byte)atoi(tokens[2].c_str());
        if (tokens.size() >= 4)
            clr.alpha = (byte)(strtod(tokens[3].c_str(), nullptr) * 255.0);

        return clr;
    } else {
        return resolve_name(str);
    }
}

bool Color::is_color(const char* str)
{
    if (!t_strncasecmp(str, "rgb", 3) || str[0] == '#') {
        return true;
    }
    if (!isdigit(str[0]) && str[0] != '.') {
        return true;
    }

    return false;
}

} // namespace litehtml
