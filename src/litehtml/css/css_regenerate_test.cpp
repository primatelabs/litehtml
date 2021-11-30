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

#include "litehtml/css/css_regenerate.h"

#include <gtest/gtest.h>

#include "litehtml/css/css_tokenizer.h"
#include "litehtml/debug/json.h"

using namespace litehtml;

TEST(CSSRegenerateTest, Smoke)
{
    std::vector<String> testcases = {
        "body { "
        "margin: 25px; "
        "background-color: rgb(220,230,240); "
        "color: #ff00ff; "
        "font-family: roboto, arial, sans-serif; "
        "font-size: 14px !important; "
        "cursor: url(mycursor.cur); "
        "}",
        "foo > bar { "
        "margin: 25px; "
        "background-color: rgb(220,230,240); "
        "color: #ff00ff; "
        "font-family: roboto, arial, sans-serif; "
        "font-size: 14px !important; "
        "}",
        "@media (color) { "
        "body { "
        "margin: 50px; "
        "} "
        "}",

    };

    for (String& testcase : testcases) {
        CSSTokenizer tokenizer(testcase);
        std::vector<CSSToken*> tokens = tokenizer.tokens();
        String result = css_regenerate(tokens);
        EXPECT_EQ(testcase, result) << result;
    }
}
