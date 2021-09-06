// Copyright (C) 2020 Primate Labs Inc. All rights reserved.
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
//    * Neither the name of Primate Labs Inc. nor the names of its
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

#include "litehtml/css_tokenizer.h"

#include <iostream>

#include <gtest/gtest.h>

using namespace litehtml;

namespace {

struct CSSTokenizerTestCase {
    tstring css;
    std::vector<css_token> tokens;
};

void test(std::vector<CSSTokenizerTestCase>& testcases) {
    for (auto testcase : testcases) {
        css_tokenizer tokenizer(testcase.css);
        auto tokens = tokenizer.tokens();
        std::cout << tokens.size() << " " << testcase.tokens.size() << std::endl;
        EXPECT_EQ(testcase.tokens.size(), tokens.size() - 1);
        for (int i = 0; i < testcase.tokens.size(); i++ ) {
            std::cout << i << " " << tokens[i].type() << " " << testcase.tokens[i].type() << std::endl;
            EXPECT_EQ(testcase.tokens[i].type(), tokens[i].type());
        }
        EXPECT_EQ(kCSSTokenEOF, tokens.back().type());
    }
}

} // namespace


#define T(t) css_token(kCSSToken ## t)

TEST(CSSTokenizerTest, DISABLED_BadString)
{
    std::vector<CSSTokenizerTestCase> testcases = {
        { _t(""), {} },
        { _t("'hi"), { T(BadString) } },
        { _t("\"hi\\"), { T(BadString) } },
        { _t("\"hi\\\""), { T(BadString) } },
    };

    test(testcases);
}

TEST(CSSTokenizerTest, Comment)
{
    std::vector<CSSTokenizerTestCase> testcases = {
        { _t(""), {} },
        { _t("/**/"), {} },
        { _t(" /* */"), { T(Whitespace) } },
        { _t("/* */ "), { T(Whitespace) } },
        { _t(" /* */ "), { T(Whitespace), T(Whitespace) } },
    };

    test(testcases);
}

TEST(CSSTokenizerTest, Number)
{
    std::vector<CSSTokenizerTestCase> testcases = {
        { _t(""), {} },
        { _t("0"), { T(Number) } },
        { _t("+0"), { T(Number) } },
        { _t("-0"), { T(Number) } },
        { _t("100"), { T(Number) } },
        { _t("100.0"), { T(Number) } },
    };

    test(testcases);
}

TEST(CSSTokenizerTest, String)
{
    std::vector<CSSTokenizerTestCase> testcases = {
        { _t(""), {} },
        { _t("'hi'"), { T(String) } },
        { _t("\"hi\""), { T(String) } },
        { _t("\"hi\\\"\""), { T(String) } },
        { _t("'hi there'"), { T(String) } },
        { _t("'hi' 'there'"), { T(String), T(Whitespace), T(String) } },
        { _t("'hi\"' 'there'"), { T(String), T(Whitespace), T(String) } },
        { _t("\"hi\'\" 'there'"), { T(String), T(Whitespace), T(String) } },
        { _t("\"hi\\\"\" 'there'"), { T(String), T(Whitespace), T(String) } },
    };

    test(testcases);
}

TEST(CSSTokenizerTest, Whitespace)
{
    std::vector<CSSTokenizerTestCase> testcases = {
        { _t(""), {} },
        { _t(" "), { T(Whitespace) } },
        { _t("\t"), { T(Whitespace) } },
        { _t("\n"), { T(Whitespace) } },
        { _t("\r"), { T(Whitespace) } },
        { _t("\f"), { T(Whitespace) } },
        { _t("  "), { T(Whitespace) } },
        { _t("  \t"), { T(Whitespace) } },
        { _t("  \n"), { T(Whitespace) } },
        { _t("  \r"), { T(Whitespace) } },
        { _t("  \f"), { T(Whitespace) } },
        { _t(" \t "), { T(Whitespace) } },
        { _t(" \n "), { T(Whitespace) } },
        { _t(" \r "), { T(Whitespace) } },
        { _t(" \f "), { T(Whitespace) } },
        { _t("\t  "), { T(Whitespace) } },
        { _t("\n  "), { T(Whitespace) } },
        { _t("\r  "), { T(Whitespace) } },
        { _t("\f  "), { T(Whitespace) } },
        { _t("\t  \n  \t  \r  \f  "), { T(Whitespace) } },
        { _t("\n  \n  \t  \r  \f  "), { T(Whitespace) } },
        { _t("\r  \n  \t  \r  \f  "), { T(Whitespace) } },
        { _t("\f  \n  \t  \r  \f  "), { T(Whitespace) } },
    };

    test(testcases);
}
