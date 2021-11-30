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

#include "litehtml/css/css_tokenizer.h"

#include <gtest/gtest.h>

#include "litehtml/logging.h"

using namespace litehtml;

namespace {

struct CSSTokenizerTestCase {
    tstring css;
    std::vector<CSSToken> tokens;
};

void test(std::vector<CSSTokenizerTestCase>& testcases)
{
    for (auto testcase : testcases) {
        CSSTokenizer tokenizer(testcase.css);
        auto tokens = tokenizer.tokens();
        EXPECT_EQ(testcase.tokens.size(), tokens.size() - 1);
        for (int i = 0; i < testcase.tokens.size(); i++) {
            auto& token = tokens[i];
            auto& reference = testcase.tokens[i];
#if defined(LITEHTML_UTF8)
            LOG(INFO) << i << " " << css_token_type_string(token->type()) << " "
                      << css_token_type_string(reference.type());
#endif
            EXPECT_EQ(reference.type(), token->type());

            if (reference.type() == kCSSTokenNumber) {
#if defined(LITEHTML_UTF8)
                LOG(INFO) << reference.numeric_value().value();
                LOG(INFO) << token->numeric_value().value();
#endif
                EXPECT_EQ(reference.numeric_value().type(),
                    token->numeric_value().type());
                EXPECT_EQ(reference.numeric_value().value(),
                    token->numeric_value().value());
            } else if (!reference.value().empty()) {
#if defined(LITEHTML_UTF8)
                LOG(INFO) << reference.value();
                LOG(INFO) << token->value();
#endif
                EXPECT_EQ(reference.value(), token->value());
            }
        }
        EXPECT_EQ(kCSSTokenEOF, tokens.back()->type());
    }
}

} // namespace


// Generate a CSS token of type t that does not contain a value.
#define T(t) CSSToken(kCSSToken##t)

// Generate a CSS token of type t that contains a string value.  This includes
// string tokens and identifier tokens.
#define TS(t, v) CSSToken(kCSSToken##t, _t(v))

// Generate a CSS number token that contains an integer value.
#define TI(v) CSSToken(kCSSTokenNumber, CSSNumber(kCSSIntegerValue, v))

// Generate a CSS number token that contains a real number value.
#define TN(v) CSSToken(kCSSTokenNumber, CSSNumber(kCSSNumberValue, v))

TEST(CSSTokenizerTest, DISABLED_BadString)
{
    std::vector<CSSTokenizerTestCase> testcases = {
        {_t(""), {}},
        {_t("'hi"), {T(BadString)}},
        {_t("\"hi\\"), {T(BadString)}},
        {_t("\"hi\\\""), {T(BadString)}},
    };

    test(testcases);
}

TEST(CSSTokenizerTest, Comment)
{
    std::vector<CSSTokenizerTestCase> testcases = {
        {_t(""), {}},
        {_t("/**/"), {}},
        {_t(" /* */"), {T(Whitespace)}},
        {_t("/* */ "), {T(Whitespace)}},
        {_t(" /* */ "), {T(Whitespace), T(Whitespace)}},
    };

    test(testcases);
}

TEST(CSSTokenizerTest, Number)
{
    std::vector<CSSTokenizerTestCase> testcases = {
        {_t(""), {}},
        {_t("0"), {TI(0)}},
        {_t("+0"), {TI(0)}},
        {_t("-0"), {TI(0)}},
        {_t("100"), {TI(100)}},
        {_t("100.0"), {TN(100.0)}},
        {_t("+.4"), {TN(0.4)}},
        {_t("-.4"), {TN(-0.4)}},
    };

    test(testcases);
}

TEST(CSSTokenizerTest, NumberSign)
{
    std::vector<CSSTokenizerTestCase> testcases = {
        {_t(""), {}},
        {_t("#ff00ff"), {TS(Hash, "ff00ff")}},
        {_t("#fff"), {TS(Hash, "fff")}},
        {_t("#000"), {TS(Hash, "000")}},
    };

    test(testcases);
}

TEST(CSSTokenizerTest, String)
{
    std::vector<CSSTokenizerTestCase> testcases = {
        {_t(""), {}},
        {_t("'hi'"), {TS(String, "hi")}},
        {_t("\"hi\""), {TS(String, "hi")}},
        {_t("\"hi\\\"\""), {TS(String, "hi\"")}},
        {_t("'hi there'"), {TS(String, "hi there")}},
        {_t("'hi' 'there'"),
            {TS(String, "hi"), T(Whitespace), TS(String, "there")}},
        {_t("'hi\"' 'there'"),
            {TS(String, "hi\""), T(Whitespace), TS(String, "there")}},
        {_t("\"hi\'\" 'there'"),
            {TS(String, "hi'"), T(Whitespace), TS(String, "there")}},
        {_t("\"hi\\\"\" 'there'"),
            {TS(String, "hi\""), T(Whitespace), TS(String, "there")}},
    };

    test(testcases);
}

TEST(CSSTokenizerTest, URL)
{
    std::vector<CSSTokenizerTestCase> testcases = {
        {_t("url()"), {TS(URL, "")}},
        {_t("url( )"), {TS(URL, "")}},
        {_t("url(https://example.com/images/myImg.jpg)"),
            {TS(URL, "https://example.com/images/myImg.jpg")}},
        {_t("url(data:image/png;base64,iRxVB0)"),
            {TS(URL, "data:image/png;base64,iRxVB0")}},
        {_t("url(#IDofSVGpath)"), {TS(URL, "#IDofSVGpath")}},
        {_t("url(myFont.woff)"), {TS(URL, "myFont.woff")}},
        {_t("url( myFont.woff)"), {TS(URL, "myFont.woff")}},
        {_t("url(myFont.woff )"), {TS(URL, "myFont.woff")}},
        {_t("url( myFont.woff )"), {TS(URL, "myFont.woff")}},
    };

    test(testcases);
}

TEST(CSSTokenizerTest, Whitespace)
{
    std::vector<CSSTokenizerTestCase> testcases = {
        {_t(""), {}},
        {_t(" "), {T(Whitespace)}},
        {_t("\t"), {T(Whitespace)}},
        {_t("\n"), {T(Whitespace)}},
        {_t("\r"), {T(Whitespace)}},
        {_t("\f"), {T(Whitespace)}},
        {_t("  "), {T(Whitespace)}},
        {_t("  \t"), {T(Whitespace)}},
        {_t("  \n"), {T(Whitespace)}},
        {_t("  \r"), {T(Whitespace)}},
        {_t("  \f"), {T(Whitespace)}},
        {_t(" \t "), {T(Whitespace)}},
        {_t(" \n "), {T(Whitespace)}},
        {_t(" \r "), {T(Whitespace)}},
        {_t(" \f "), {T(Whitespace)}},
        {_t("\t  "), {T(Whitespace)}},
        {_t("\n  "), {T(Whitespace)}},
        {_t("\r  "), {T(Whitespace)}},
        {_t("\f  "), {T(Whitespace)}},
        {_t("\t  \n  \t  \r  \f  "), {T(Whitespace)}},
        {_t("\n  \n  \t  \r  \f  "), {T(Whitespace)}},
        {_t("\r  \n  \t  \r  \f  "), {T(Whitespace)}},
        {_t("\f  \n  \t  \r  \f  "), {T(Whitespace)}},
    };

    test(testcases);
}

TEST(CSSTokenizerTest, Stylesheet)
{
    std::vector<CSSTokenizerTestCase> testcases = {
        {_t("/* A simple CSS stylesheet */\n"
            "body {\n"
            "  margin: 25px;\n"
            "  background-color: rgb(220,230,240);\n"
            "  font-family: roboto, arial, sans-serif;\n"
            "  font-size: 14px !important;\n"
            "}\n"),
            {T(Whitespace),
                TS(Ident, "body"),
                T(Whitespace),
                T(OpenBrace),
                T(Whitespace),
                TS(Ident, "margin"),
                T(Colon),
                T(Whitespace),
                TI(25),
                TS(Ident, "px"),
                T(Semicolon),
                T(Whitespace),
                TS(Ident, "background-color"),
                T(Colon),
                T(Whitespace),
                TS(Function, "rgb"),
                TI(220),
                T(Comma),
                TI(230),
                T(Comma),
                TI(240),
                T(CloseRoundBracket),
                T(Semicolon),
                T(Whitespace),
                TS(Ident, "font-family"),
                T(Colon),
                T(Whitespace),
                TS(Ident, "roboto"),
                T(Comma),
                T(Whitespace),
                TS(Ident, "arial"),
                T(Comma),
                T(Whitespace),
                TS(Ident, "sans-serif"),
                T(Semicolon),
                T(Whitespace),
                TS(Ident, "font-size"),
                T(Colon),
                T(Whitespace),
                TI(14),
                TS(Ident, "px"),
                T(Whitespace),
                TS(Delim, "!"),
                TS(Ident, "important"),
                T(Semicolon),
                T(Whitespace),
                T(CloseBrace),
                T(Whitespace)}},
        {_t("body {\n"
            "  margin: 25px;\n"
            "}\n"
            "@media (color) {\n"
            "  body {\n"
            "    margin: 50px;\n"
            "  }\n"
            "}\n"),
            {TS(Ident, "body"),
                T(Whitespace),
                T(OpenBrace),
                T(Whitespace),
                TS(Ident, "margin"),
                T(Colon),
                T(Whitespace),
                TI(25),
                TS(Ident, "px"),
                T(Semicolon),
                T(Whitespace),
                T(CloseBrace),
                T(Whitespace),
                TS(AtKeyword, "media"),
                T(Whitespace),
                T(OpenRoundBracket),
                TS(Ident, "color"),
                T(CloseRoundBracket),
                T(Whitespace),
                T(OpenBrace),
                T(Whitespace),
                TS(Ident, "body"),
                T(Whitespace),
                T(OpenBrace),
                T(Whitespace),
                TS(Ident, "margin"),
                T(Colon),
                T(Whitespace),
                TI(50),
                TS(Ident, "px"),
                T(Semicolon),
                T(Whitespace),
                T(CloseBrace),
                T(Whitespace),
                T(CloseBrace),
                T(Whitespace)}}};

    test(testcases);
}
