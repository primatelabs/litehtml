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

#include "litehtml/color.h"

#include <gtest/gtest.h>

using namespace litehtml;

namespace {

struct ParseTestcase {
  std::string str;
  Color expected;
};

} // namespace

TEST(ColorTest, DefaultConstructor)
{
    Color color;

    EXPECT_EQ(0, color.red);
    EXPECT_EQ(0, color.green);
    EXPECT_EQ(0, color.blue);
    EXPECT_EQ(255, color.alpha);

}

TEST(ColorTest, Parse)
{
    std::vector<ParseTestcase> testcases = {
        {"",     Color(0, 0, 0, 255)},

        {"#000", Color(  0,   0,   0, 255)},
        {"#00f", Color(  0,   0, 255, 255)},
        {"#0f0", Color(  0, 255,   0, 255)},
        {"#0ff", Color(  0, 255, 255, 255)},
        {"#f00", Color(255,   0,   0, 255)},
        {"#f0f", Color(255,   0, 255, 255)},
        {"#ff0", Color(255, 255,   0, 255)},
        {"#fff", Color(255, 255, 255, 255)},

        {"#000", Color(  0,   0,   0, 255)},
        {"#003", Color(  0,   0,  51, 255)},
        {"#030", Color(  0,  51,   0, 255)},
        {"#033", Color(  0,  51,  51, 255)},
        {"#300", Color( 51,   0,   0, 255)},
        {"#303", Color( 51,   0,  51, 255)},
        {"#330", Color( 51,  51,   0, 255)},
        {"#333", Color( 51,  51,  51, 255)},

        {"#000000", Color(  0,   0,   0, 255)},
        {"#0000ff", Color(  0,   0, 255, 255)},
        {"#00ff00", Color(  0, 255,   0, 255)},
        {"#00ffff", Color(  0, 255, 255, 255)},
        {"#ff0000", Color(255,   0,   0, 255)},
        {"#ff00ff", Color(255,   0, 255, 255)},
        {"#ffff00", Color(255, 255,   0, 255)},
        {"#ffffff", Color(255, 255, 255, 255)},

        {"#000000", Color(  0,   0,   0, 255)},
        {"#000033", Color(  0,   0,  51, 255)},
        {"#003300", Color(  0,  51,   0, 255)},
        {"#003333", Color(  0,  51,  51, 255)},
        {"#330000", Color( 51,   0,   0, 255)},
        {"#330033", Color( 51,   0,  51, 255)},
        {"#333300", Color( 51,  51,   0, 255)},
        {"#333333", Color( 51,  51,  51, 255)},

        {"rgb()", Color(0, 0, 0, 255)},

        {"rgb(  0,   0,   0)", Color(  0,   0,   0, 255)},
        {"rgb(  0,   0, 255)", Color(  0,   0, 255, 255)},
        {"rgb(  0, 255,   0)", Color(  0, 255,   0, 255)},
        {"rgb(  0, 255, 255)", Color(  0, 255, 255, 255)},
        {"rgb(255,   0,   0)", Color(255,   0,   0, 255)},
        {"rgb(255,   0, 255)", Color(255,   0, 255, 255)},
        {"rgb(255, 255,   0)", Color(255, 255,   0, 255)},
        {"rgb(255, 255, 255)", Color(255, 255, 255, 255)},

        {"unknown",       Color(  0,   0,   0, 255)},

        {"black",         Color(  0,   0,   0, 255)},
        {"darkgoldenrod", Color(184, 134,  11, 255)},
        {"ghostwhite",    Color(248, 248, 255, 255)},
        {"gray",          Color(128, 128, 128, 255)},
        {"red",           Color(255,   0,   0, 255)},
        {"silver",        Color(192, 192, 192, 255)},
        {"transparent",   Color(  0,   0,   0,   0)},
    };

    for (auto& testcase : testcases) {
        Color color = Color::from_string(testcase.str.c_str());
        EXPECT_EQ(testcase.expected.red, color.red) << testcase.str;
        EXPECT_EQ(testcase.expected.green, color.green) << testcase.str;
        EXPECT_EQ(testcase.expected.blue, color.blue) << testcase.str;
        EXPECT_EQ(testcase.expected.alpha, color.alpha) << testcase.str;
    }
}
