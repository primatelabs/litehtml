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

#include "litehtml/web_color.h"

#include <gtest/gtest.h>

using namespace litehtml;

namespace {

struct ParseTestcase {
  std::string str;
  WebColor expected;
};

} // namespace

TEST(WebColorTest, DefaultConstructor)
{
    WebColor color;

    EXPECT_EQ(0, color.red);
    EXPECT_EQ(0, color.green);
    EXPECT_EQ(0, color.blue);
    EXPECT_EQ(255, color.alpha);

}

TEST(WebColorTest, Parse)
{
    std::vector<ParseTestcase> testcases = {
        {"",     WebColor(0, 0, 0, 255)},

        {"#000", WebColor(  0,   0,   0, 255)},
        {"#00f", WebColor(  0,   0, 255, 255)},
        {"#0f0", WebColor(  0, 255,   0, 255)},
        {"#0ff", WebColor(  0, 255, 255, 255)},
        {"#f00", WebColor(255,   0,   0, 255)},
        {"#f0f", WebColor(255,   0, 255, 255)},
        {"#ff0", WebColor(255, 255,   0, 255)},
        {"#fff", WebColor(255, 255, 255, 255)},

        {"#000", WebColor(  0,   0,   0, 255)},
        {"#003", WebColor(  0,   0,  51, 255)},
        {"#030", WebColor(  0,  51,   0, 255)},
        {"#033", WebColor(  0,  51,  51, 255)},
        {"#300", WebColor( 51,   0,   0, 255)},
        {"#303", WebColor( 51,   0,  51, 255)},
        {"#330", WebColor( 51,  51,   0, 255)},
        {"#333", WebColor( 51,  51,  51, 255)},

        {"#000000", WebColor(  0,   0,   0, 255)},
        {"#0000ff", WebColor(  0,   0, 255, 255)},
        {"#00ff00", WebColor(  0, 255,   0, 255)},
        {"#00ffff", WebColor(  0, 255, 255, 255)},
        {"#ff0000", WebColor(255,   0,   0, 255)},
        {"#ff00ff", WebColor(255,   0, 255, 255)},
        {"#ffff00", WebColor(255, 255,   0, 255)},
        {"#ffffff", WebColor(255, 255, 255, 255)},

        {"#000000", WebColor(  0,   0,   0, 255)},
        {"#000033", WebColor(  0,   0,  51, 255)},
        {"#003300", WebColor(  0,  51,   0, 255)},
        {"#003333", WebColor(  0,  51,  51, 255)},
        {"#330000", WebColor( 51,   0,   0, 255)},
        {"#330033", WebColor( 51,   0,  51, 255)},
        {"#333300", WebColor( 51,  51,   0, 255)},
        {"#333333", WebColor( 51,  51,  51, 255)},

        {"rgb()", WebColor(0, 0, 0, 255)},

        {"rgb(  0,   0,   0)", WebColor(  0,   0,   0, 255)},
        {"rgb(  0,   0, 255)", WebColor(  0,   0, 255, 255)},
        {"rgb(  0, 255,   0)", WebColor(  0, 255,   0, 255)},
        {"rgb(  0, 255, 255)", WebColor(  0, 255, 255, 255)},
        {"rgb(255,   0,   0)", WebColor(255,   0,   0, 255)},
        {"rgb(255,   0, 255)", WebColor(255,   0, 255, 255)},
        {"rgb(255, 255,   0)", WebColor(255, 255,   0, 255)},
        {"rgb(255, 255, 255)", WebColor(255, 255, 255, 255)},

        {"unknown",       WebColor(  0,   0,   0, 255)},

        {"black",         WebColor(  0,   0,   0, 255)},
        {"darkgoldenrod", WebColor(184, 134,  11, 255)},
        {"ghostwhite",    WebColor(248, 248, 255, 255)},
        {"gray",          WebColor(128, 128, 128, 255)},
        {"red",           WebColor(255,   0,   0, 255)},
        {"silver",        WebColor(192, 192, 192, 255)},
        {"transparent",   WebColor(  0,   0,   0,   0)},
    };

    for (auto& testcase : testcases) {
        WebColor color = WebColor::from_string(testcase.str.c_str());
        EXPECT_EQ(testcase.expected.red, color.red) << testcase.str;
        EXPECT_EQ(testcase.expected.green, color.green) << testcase.str;
        EXPECT_EQ(testcase.expected.blue, color.blue) << testcase.str;
        EXPECT_EQ(testcase.expected.alpha, color.alpha) << testcase.str;
    }
}
