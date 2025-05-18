// Copyright (c) 2013, Yuri Kobets (tordex)
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

#include <assert.h>
#include <gtest/gtest.h>

#include "litehtml/litehtml.h"

#include "test_container.h"

using namespace litehtml;

#if 0
TEST(CSSTest, Parse)
{
    test_container container;
    litehtml::Document::ptr doc =
        std::make_shared<litehtml::document>(&container, nullptr);
    MediaQueryList::ptr media = MediaQueryList::ptr();
    CSSStylesheet c;
    c.parse_stylesheet("/*Comment*/", URL(), doc, nullptr);
    c.parse_stylesheet("html { display: none }", URL(), doc, nullptr);
    // https://www.w3schools.com/cssref/pr_import_rule.asp
    c.parse_stylesheet(_t("@import \"navigation.css\"; /* Using a string */"),
        URL(),
        doc,
        nullptr);
    c.parse_stylesheet(_t("@import url(\"navigation.css\"); /* Using a url */"),
        URL(),
        doc,
        nullptr);
    c.parse_stylesheet(_t("@import \"navigation.css\""), URL(), doc, nullptr);
    c.parse_stylesheet(_t("@import \"printstyle.css\" print;"), URL(), doc, nullptr);
    c.parse_stylesheet(
        _t("@import \"mobstyle.css\" screen and (max-width: 768px);"),
        URL(),
        doc,
        nullptr);
    // https://www.w3schools.com/cssref/css3_pr_mediaquery.asp
    c.parse_stylesheet(_t("@media only screen and (max-width: 600px) { body { "
                          "background-color: lightblue; } }"),
        URL(),
        doc,
        nullptr);
}

TEST(CSSTest, Url)
{
    std::string url;

    CSSStylesheet::parse_css_url("", url);
    EXPECT_TRUE(url.empty());

    CSSStylesheet::parse_css_url("value", url);
    EXPECT_TRUE(url.empty());

    CSSStylesheet::parse_css_url("url()", url);
    EXPECT_TRUE(url.empty());

    CSSStylesheet::parse_css_url("url(value)", url);
    EXPECT_TRUE(!strcmp(url.c_str(), "value"));

    CSSStylesheet::parse_css_url("url('value')", url);
    EXPECT_TRUE(!strcmp(url.c_str(), "value"));

    CSSStylesheet::parse_css_url(_t("url(\"value\")"), url);
    EXPECT_TRUE(!strcmp(url.c_str(), "value"));
}

TEST(CSSTest, LengthParse)
{
    CSSLength length;

    length.fromString("calc(todo)");
    assert(length.is_predefined() == true);
    assert(length.predef() == 0);
    assert(length.val() == 0);
    assert(length.units() == kCSSUnitsNone);

    length.fromString(kCSSPropertyTop, "top;bottom", -1);
    assert(length.is_predefined() == true);
    assert(length.predef() == 0);
    assert(length.val() == 0);
    assert(length.units() == kCSSUnitsNone);

    length.fromString(kCSSPropertyBottom, "top;bottom", -1);
    assert(length.is_predefined() == true);
    assert(length.predef() == 1);
    assert(length.val() == 0);
    assert(length.units() == kCSSUnitsNone);

    length.fromString("bad", "top;bottom", -1);
    assert(length.is_predefined() == true);
    assert(length.predef() == -1);
    assert(length.val() == 0);
    assert(length.units() == kCSSUnitsNone);

    length.fromString("123", "top;bottom", -1);
    assert(length.is_predefined() == false);
    assert(length.predef() == 0);
    assert(length.val() == 123);
    assert(length.units() == kCSSUnitsNone);

    length.fromString("123px", "top;bottom", -1);
    assert(length.is_predefined() == false);
    assert(length.predef() == 0);
    assert(length.val() == 123);
    assert(length.units() == kCSSUnitsPx);
}

TEST(CSSTest, StyleAdd)
{
    style style;
    style.add("border: 5px solid red; background-image: value", "base");
    style.add("border: 5px solid red!important; background-image: value",
        "base");
}

TEST(CSSTest, StyleAddProperty)
{
    style style;
    style.add_property(kCSSPropertyBackgroundImage, "value", "base", false);
    style.add_property(kCSSPropertyBorderSpacing, "1", nullptr, false);
    style.add_property(kCSSPropertyBorderSpacing, "1 2", nullptr, false);
    style.add_property(kCSSPropertyBorder, "5px solid red", nullptr, false);
    style.add_property(kCSSPropertyBorderLeft, "5px solid red", nullptr, false);
    style.add_property(kCSSPropertyBorderRight, "5px solid red", nullptr, false);
    style.add_property(kCSSPropertyBorderTop, "5px solid red", nullptr, false);
    style.add_property(kCSSPropertyBorderBottom, "5px solid red", nullptr, false);
    style.add_property(kCSSPropertyBorderBottomLeftRadius, "1", nullptr, false);
    style.add_property(kCSSPropertyBorderBottomLeftRadius, "1 2", nullptr, false);
    style.add_property(kCSSPropertyBorderBottomRightRadius, "1", nullptr, false);
    style.add_property(kCSSPropertyBorderBottomRightRadius, "1 2", nullptr, false);
    style.add_property(kCSSPropertyBorderTopRightRadius, "1", nullptr, false);
    style.add_property(kCSSPropertyBorderTopRightRadius, "1 2", nullptr, false);
    style.add_property(kCSSPropertyBorderTopLeftRadius, "1", nullptr, false);
    style.add_property(kCSSPropertyBorderTopLeftRadius, "1 2", nullptr, false);
    style.add_property(kCSSPropertyBorderRadius, "1", nullptr, false);
    style.add_property(kCSSPropertyBorderRadius, "1 2", nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusX, "1", nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusX, "1 2", nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusX, "1 2 3", nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusX, "1 2 3 4", nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusY, "1", nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusY, "1 2", nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusY, "1 2 3", nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusY, "1 2 3 4", nullptr, false);
    style.add_property(kCSSPropertyListStyleImage, "value", "base", false);
    style.add_property(kCSSPropertyBackground, "url(value)", "base", false);
    style.add_property(kCSSPropertyBackground, "repeat", nullptr, false);
    style.add_property(kCSSPropertyBackground, "fixed", nullptr, false);
    style.add_property(kCSSPropertyBackground, "border-box", nullptr, false);
    style.add_property(kCSSPropertyBackground, "border-box border-box", nullptr, false);
    style.add_property(kCSSPropertyBackground, kCSSPropertyLeft, nullptr, false);
    style.add_property(kCSSPropertyBackground, "1", nullptr, false);
    style.add_property(kCSSPropertyBackground, "-1", nullptr, false);
    style.add_property(kCSSPropertyBackground, "-1", nullptr, false);
    style.add_property(kCSSPropertyBackground, "+1", nullptr, false);
    style.add_property(kCSSPropertyBackground, "left 1", nullptr, false);
    style.add_property(kCSSPropertyBackground, "red", nullptr, false);
    style.add_property(kCSSPropertyMargin, "1", nullptr, false);
    style.add_property(kCSSPropertyMargin, "1 2", nullptr, false);
    style.add_property(kCSSPropertyMargin, "1 2 3", nullptr, false);
    style.add_property(kCSSPropertyMargin, "1 2 3 4", nullptr, false);
    style.add_property(kCSSPropertyPadding, "1", nullptr, false);
    style.add_property(kCSSPropertyPadding, "1 2", nullptr, false);
    style.add_property(kCSSPropertyPadding, "1 2 3", nullptr, false);
    style.add_property(kCSSPropertyPadding, "1 2 3 4", nullptr, false);
    style.add_property(kCSSPropertyBorderLeft, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderLeft, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderLeft, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderLeft, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderRight, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderRight, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderRight, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderRight, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderTop, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderTop, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderTop, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderTop, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderBottom, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderBottom, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderBottom, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderBottom, "TBD", nullptr, false);
    style.add_property(kCSSPropertyBorderWidth, "1", nullptr, false);
    style.add_property(kCSSPropertyBorderWidth, "1 2", nullptr, false);
    style.add_property(kCSSPropertyBorderWidth, "1 2 3", nullptr, false);
    style.add_property(kCSSPropertyBorderWidth, "1 2 3 4", nullptr, false);
    style.add_property(kCSSPropertyBorderStyle, "1", nullptr, false);
    style.add_property(kCSSPropertyBorderStyle, "1 2", nullptr, false);
    style.add_property(kCSSPropertyBorderStyle, "1 2 3", nullptr, false);
    style.add_property(kCSSPropertyBorderStyle, "1 2 3 4", nullptr, false);
    style.add_property(kCSSPropertyBorderColor, "1", nullptr, false);
    style.add_property(kCSSPropertyBorderColor, "1 2", nullptr, false);
    style.add_property(kCSSPropertyBorderColor, "1 2 3", nullptr, false);
    style.add_property(kCSSPropertyBorderColor, "1 2 3 4", nullptr, false);
    style.add_property(kCSSPropertyFont, "TBD", nullptr, false);
    style.add_property(kCSSPropertyFont, "TBD", nullptr, false);
    style.add_property(kCSSPropertyFont, "TBD", nullptr, false);
    style.add_property(kCSSPropertyFont, "TBD", nullptr, false);
    style.add_property("unknown", "value", nullptr, false);
}
#endif
