// Copyright (C) 2020-2021 Primate Labs Inc. All rights reserved.
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
//    * Neither the name of the copyright holder nor the names of its
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

#include "litehtml/css/css_parser.h"

#include <gtest/gtest.h>

#include "litehtml/debug/json.h"
#include "litehtml/string.h"

using namespace litehtml;

TEST(CSSParserTest, Stylesheet)
{
    std::vector<String> testcases = {
        "/* A simple CSS stylesheet */\n"
        "body {\n"
        "  margin: 25px;\n"
        "  background-color: rgb(240,240,240);\n"
        "  font-family: roboto, arial, sans-serif;\n"
        "  font-size: 14px;\n"
        // "  color: #ff00ff !important;\n"
        "  color: #ff00ff;\n"
        "}\n",
        "body {\n"
        "  margin: 25px;\n"
        "}\n"
        "@media (color) {\n"
        "  body {\n"
        "    margin: 50px;\n"
        "  }\n"
        "}\n",
    };

    for (String& testcase : testcases) {
        CSSParser parser(testcase);
        CSSStylesheet* stylesheet = parser.parse_stylesheet();
    }
}

TEST(CSSParserTest, Fuzz)
{
    std::vector<String> testcases = {
        "/( A simple CSS stylesheet */\n",
    };

    for (String& testcase : testcases) {
        CSSParser parser(testcase);
        CSSStylesheet* stylesheet = parser.parse_stylesheet();
    }
}
