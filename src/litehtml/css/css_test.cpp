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
    c.parse_stylesheet(_t("/*Comment*/"), URL(), doc, nullptr);
    c.parse_stylesheet(_t("html { display: none }"), URL(), doc, nullptr);
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
    tstring url;

    CSSStylesheet::parse_css_url(_t(""), url);
    EXPECT_TRUE(url.empty());

    CSSStylesheet::parse_css_url(_t("value"), url);
    EXPECT_TRUE(url.empty());

    CSSStylesheet::parse_css_url(_t("url()"), url);
    EXPECT_TRUE(url.empty());

    CSSStylesheet::parse_css_url(_t("url(value)"), url);
    EXPECT_TRUE(!t_strcmp(url.c_str(), _t("value")));

    CSSStylesheet::parse_css_url(_t("url('value')"), url);
    EXPECT_TRUE(!t_strcmp(url.c_str(), _t("value")));

    CSSStylesheet::parse_css_url(_t("url(\"value\")"), url);
    EXPECT_TRUE(!t_strcmp(url.c_str(), _t("value")));
}

TEST(CSSTest, LengthParse)
{
    CSSLength length;

    length.fromString(_t("calc(todo)"));
    assert(length.is_predefined() == true);
    assert(length.predef() == 0);
    assert(length.val() == 0);
    assert(length.units() == kCSSUnitsNone);

    length.fromString(kCSSPropertyTop, _t("top;bottom"), -1);
    assert(length.is_predefined() == true);
    assert(length.predef() == 0);
    assert(length.val() == 0);
    assert(length.units() == kCSSUnitsNone);

    length.fromString(kCSSPropertyBottom, _t("top;bottom"), -1);
    assert(length.is_predefined() == true);
    assert(length.predef() == 1);
    assert(length.val() == 0);
    assert(length.units() == kCSSUnitsNone);

    length.fromString(_t("bad"), _t("top;bottom"), -1);
    assert(length.is_predefined() == true);
    assert(length.predef() == -1);
    assert(length.val() == 0);
    assert(length.units() == kCSSUnitsNone);

    length.fromString(_t("123"), _t("top;bottom"), -1);
    assert(length.is_predefined() == false);
    assert(length.predef() == 0);
    assert(length.val() == 123);
    assert(length.units() == kCSSUnitsNone);

    length.fromString(_t("123px"), _t("top;bottom"), -1);
    assert(length.is_predefined() == false);
    assert(length.predef() == 0);
    assert(length.val() == 123);
    assert(length.units() == kCSSUnitsPx);
}

TEST(CSSTest, ElementSelectorParse)
{
    CSSElementSelector selector;
    // https://www.w3schools.com/cssref/CSSSelectors.asp
    selector.parse(_t(".class"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("class")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("class")));

    selector.parse(_t(".class1.class2"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 2);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("class1")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[1].val.c_str(), _t("class2")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("class")));

    selector.parse(_t("#id"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("id")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("id")));

    selector.parse(_t("*"));
    EXPECT_TRUE(!t_strcmp(selector.m_tag.c_str(), _t("*")));
    EXPECT_TRUE(selector.m_attrs.empty());

    selector.parse(_t("element"));
    EXPECT_TRUE(!t_strcmp(selector.m_tag.c_str(), _t("element")));
    EXPECT_TRUE(selector.m_attrs.empty());

    selector.parse(_t("[attribute]"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("attribute")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectExists);

    selector.parse(_t("[attribute=value]"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("value")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("attribute")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectEqual);

    selector.parse(_t("[attribute~=value]"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("value")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("attribute")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectContainStr);

    selector.parse(_t("[attribute|=value]"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("value")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("attribute")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectStartStr);

    selector.parse(_t("[attribute^=value]"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("value")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("attribute")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectStartStr);

    selector.parse(_t("[attribute$=value]"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("value")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("attribute")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectEndStr);

    selector.parse(_t("[attribute*=value]"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("value")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("attribute")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectContainStr);

    selector.parse(_t(":active"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("active")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t("::after"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("after")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo-el")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoElement);

    selector.parse(_t("::before"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("before")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo-el")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoElement);

    selector.parse(_t(":checked"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("checked")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":default"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("default")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":disabled"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("disabled")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":empty"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("empty")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":enabled"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("enabled")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":first-child"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("first-child")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t("::first-letter"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("first-letter")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo-el")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoElement);

    selector.parse(_t("::first-line"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("first-line")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo-el")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoElement);

    selector.parse(_t(":first-of-type"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("first-of-type")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":focus"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("focus")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":hover"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("hover")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":in-range"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("in-range")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":indeterminate"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("indeterminate")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":invalid"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("invalid")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":lang(language)"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("lang(language)")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":last-child"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("last-child")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":last-of-type"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("last-of-type")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":link"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("link")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":not(selector)"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("not(selector)")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":nth-child(n)"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("nth-child(n)")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":nth-last-child(n)"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(
        !t_strcmp(selector.m_attrs[0].val.c_str(), _t("nth-last-child(n)")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":nth-last-of-type(n)"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(
        !t_strcmp(selector.m_attrs[0].val.c_str(), _t("nth-last-of-type(n)")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":nth-of-type(n)"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("nth-of-type(n)")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":only-of-type"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("only-of-type")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":only-child"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("only-child")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":optional"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("optional")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":out-of-range"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("out-of-range")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t("::placeholder"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("placeholder")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo-el")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoElement);

    selector.parse(_t(":read-only"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("read-only")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":read-write"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("read-write")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":required"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("required")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":root"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("root")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t("::selection"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("selection")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo-el")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoElement);

    selector.parse(_t(":target"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("target")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":valid"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("valid")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(_t(":visited"));
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].val.c_str(), _t("visited")));
    EXPECT_TRUE(!t_strcmp(selector.m_attrs[0].attribute.c_str(), _t("pseudo")));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    // other
    selector.parse(_t("tag:psudo#anchor"));
    EXPECT_TRUE(!t_strcmp(selector.m_tag.c_str(), _t("tag")));
    EXPECT_TRUE(selector.m_attrs.size() == 2);
}

TEST(CSSTest, DISABLED_SelectorParse)
{
    CSSSelector selector(nullptr);
    // https://www.w3schools.com/cssref/CSSSelectors.asp
    assert(!selector.parse(_t("")));
    EXPECT_TRUE(selector.parse(_t("element")));
    EXPECT_TRUE(selector.m_combinator == kCombinatorDescendant);
    EXPECT_TRUE(!t_strcmp(selector.m_right.m_tag.c_str(), _t("element")));
    EXPECT_TRUE(selector.m_right.m_attrs.empty());
    EXPECT_TRUE(selector.m_left == nullptr);

    // assert(selector.parse(_t("element,element")));
    // EXPECT_TRUE(selector.m_combinator == kCombinatorDescendant);
    // EXPECT_TRUE(selector.m_right.m_tag.c_str(), _t("element"));
    // EXPECT_TRUE(selector.m_right.m_attrs.empty());

    EXPECT_TRUE(selector.parse(_t(".class1 .class2")));
    EXPECT_TRUE(selector.m_combinator == kCombinatorDescendant);
    EXPECT_TRUE(selector.m_right.m_tag.empty());
    EXPECT_TRUE(selector.m_right.m_attrs.size() == 1);
    EXPECT_TRUE(selector.m_left->m_right.m_attrs.size() == 1);

    assert(selector.parse(_t("element element")));
    EXPECT_TRUE(selector.m_combinator == kCombinatorDescendant);
    EXPECT_TRUE(!t_strcmp(selector.m_right.m_tag.c_str(), _t("element")));
    EXPECT_TRUE(selector.m_right.m_attrs.empty());
    EXPECT_TRUE(!t_strcmp(selector.m_left->m_right.m_tag.c_str(), _t("element")));

    assert(selector.parse(_t("element>element")));
    EXPECT_TRUE(selector.m_combinator == kCombinatorChild);
    EXPECT_TRUE(!t_strcmp(selector.m_right.m_tag.c_str(), _t("element")));
    EXPECT_TRUE(selector.m_right.m_attrs.empty());
    EXPECT_TRUE(!t_strcmp(selector.m_left->m_right.m_tag.c_str(), _t("element")));

    assert(selector.parse(_t("element+element")));
    EXPECT_TRUE(selector.m_combinator == kCombinatorAdjacentSibling);
    EXPECT_TRUE(!t_strcmp(selector.m_right.m_tag.c_str(), _t("element")));
    EXPECT_TRUE(selector.m_right.m_attrs.empty());
    EXPECT_TRUE(!t_strcmp(selector.m_left->m_right.m_tag.c_str(), _t("element")));

    assert(selector.parse(_t("element1~element2")));
    EXPECT_TRUE(selector.m_combinator == kCombinatorGeneralSibling);
    EXPECT_TRUE(!t_strcmp(selector.m_right.m_tag.c_str(), _t("element2")));
    EXPECT_TRUE(selector.m_right.m_attrs.empty());
    EXPECT_TRUE(!t_strcmp(selector.m_left->m_right.m_tag.c_str(), _t("element1")));
}

TEST(CSSTest, StyleAdd)
{
    style style;
    style.add(_t("border: 5px solid red; background-image: value"), _t("base"));
    style.add(_t("border: 5px solid red!important; background-image: value"),
        _t("base"));
}

TEST(CSSTest, StyleAddProperty)
{
    style style;
    style.add_property(kCSSPropertyBackgroundImage, _t("value"), _t("base"), false);
    style.add_property(kCSSPropertyBorderSpacing, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyBorderSpacing, _t("1 2"), nullptr, false);
    style.add_property(kCSSPropertyBorder, _t("5px solid red"), nullptr, false);
    style.add_property(kCSSPropertyBorderLeft, _t("5px solid red"), nullptr, false);
    style.add_property(kCSSPropertyBorderRight, _t("5px solid red"), nullptr, false);
    style.add_property(kCSSPropertyBorderTop, _t("5px solid red"), nullptr, false);
    style.add_property(kCSSPropertyBorderBottom, _t("5px solid red"), nullptr, false);
    style.add_property(kCSSPropertyBorderBottomLeftRadius, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyBorderBottomLeftRadius, _t("1 2"), nullptr, false);
    style.add_property(kCSSPropertyBorderBottomRightRadius, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyBorderBottomRightRadius, _t("1 2"), nullptr, false);
    style.add_property(kCSSPropertyBorderTopRightRadius, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyBorderTopRightRadius, _t("1 2"), nullptr, false);
    style.add_property(kCSSPropertyBorderTopLeftRadius, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyBorderTopLeftRadius, _t("1 2"), nullptr, false);
    style.add_property(kCSSPropertyBorderRadius, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyBorderRadius, _t("1 2"), nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusX, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusX, _t("1 2"), nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusX, _t("1 2 3"), nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusX, _t("1 2 3 4"), nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusY, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusY, _t("1 2"), nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusY, _t("1 2 3"), nullptr, false);
    style.add_property(kCSSPropertyBorderRadiusY, _t("1 2 3 4"), nullptr, false);
    style.add_property(kCSSPropertyListStyleImage, _t("value"), _t("base"), false);
    style.add_property(kCSSPropertyBackground, _t("url(value)"), _t("base"), false);
    style.add_property(kCSSPropertyBackground, _t("repeat"), nullptr, false);
    style.add_property(kCSSPropertyBackground, _t("fixed"), nullptr, false);
    style.add_property(kCSSPropertyBackground, _t("border-box"), nullptr, false);
    style.add_property(kCSSPropertyBackground, _t("border-box border-box"), nullptr, false);
    style.add_property(kCSSPropertyBackground, kCSSPropertyLeft, nullptr, false);
    style.add_property(kCSSPropertyBackground, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyBackground, _t("-1"), nullptr, false);
    style.add_property(kCSSPropertyBackground, _t("-1"), nullptr, false);
    style.add_property(kCSSPropertyBackground, _t("+1"), nullptr, false);
    style.add_property(kCSSPropertyBackground, _t("left 1"), nullptr, false);
    style.add_property(kCSSPropertyBackground, _t("red"), nullptr, false);
    style.add_property(kCSSPropertyMargin, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyMargin, _t("1 2"), nullptr, false);
    style.add_property(kCSSPropertyMargin, _t("1 2 3"), nullptr, false);
    style.add_property(kCSSPropertyMargin, _t("1 2 3 4"), nullptr, false);
    style.add_property(kCSSPropertyPadding, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyPadding, _t("1 2"), nullptr, false);
    style.add_property(kCSSPropertyPadding, _t("1 2 3"), nullptr, false);
    style.add_property(kCSSPropertyPadding, _t("1 2 3 4"), nullptr, false);
    style.add_property(kCSSPropertyBorderLeft, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderLeft, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderLeft, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderLeft, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderRight, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderRight, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderRight, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderRight, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderTop, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderTop, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderTop, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderTop, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderBottom, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderBottom, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderBottom, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderBottom, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyBorderWidth, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyBorderWidth, _t("1 2"), nullptr, false);
    style.add_property(kCSSPropertyBorderWidth, _t("1 2 3"), nullptr, false);
    style.add_property(kCSSPropertyBorderWidth, _t("1 2 3 4"), nullptr, false);
    style.add_property(kCSSPropertyBorderStyle, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyBorderStyle, _t("1 2"), nullptr, false);
    style.add_property(kCSSPropertyBorderStyle, _t("1 2 3"), nullptr, false);
    style.add_property(kCSSPropertyBorderStyle, _t("1 2 3 4"), nullptr, false);
    style.add_property(kCSSPropertyBorderColor, _t("1"), nullptr, false);
    style.add_property(kCSSPropertyBorderColor, _t("1 2"), nullptr, false);
    style.add_property(kCSSPropertyBorderColor, _t("1 2 3"), nullptr, false);
    style.add_property(kCSSPropertyBorderColor, _t("1 2 3 4"), nullptr, false);
    style.add_property(kCSSPropertyFont, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyFont, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyFont, _t("TBD"), nullptr, false);
    style.add_property(kCSSPropertyFont, _t("TBD"), nullptr, false);
    style.add_property(_t("unknown"), _t("value"), nullptr, false);
}
#endif
