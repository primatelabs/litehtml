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

#include "litehtml/css/css_length.h"

#include <gtest/gtest.h>

using namespace litehtml;

namespace {

struct CSSLengthTestcase {
    String str;
    String keywords;
    int default_keyword;
    CSSLength expected;
};

} // namespace;

TEST(CSSLength, FromString)
{
    std::vector<CSSLengthTestcase> testcases = {
        {"auto", "auto", 0, CSSLength(0)},
        {"none", "none", 0, CSSLength(0)},

        {"thin", "thin;medium;thick", 0, CSSLength(0)},
        {"medium", "thin;medium;thick", 0, CSSLength(1)},
        {"thick", "thin;medium;thick", 0, CSSLength(2)},
        {"", "thin;medium;thick", 0, CSSLength(0)},

        {"large", "xx-small;x-small;small;medium;large;x-large;xx-large;smaller;larger", 0, CSSLength(4)},
        {"14px", "xx-small;x-small;small;medium;large;x-large;xx-large;smaller;larger", 0, CSSLength(14, kCSSUnitsPx)},

        {"18px", "auto", 0, CSSLength(18, kCSSUnitsPx)},
        {"100%", "auto", 0, CSSLength(100, kCSSUnitsPercent)},
    };

    for (auto& testcase : testcases) {
        CSSLength length;
        length.parse_length_string(testcase.str, testcase.keywords, testcase.default_keyword);
        EXPECT_EQ(testcase.expected.is_predefined(), length.is_predefined());
        EXPECT_EQ(testcase.expected.predef(), length.predef());
        EXPECT_EQ(testcase.expected.val(), length.val());
        EXPECT_EQ(testcase.expected.units(), length.units());
    }
}
