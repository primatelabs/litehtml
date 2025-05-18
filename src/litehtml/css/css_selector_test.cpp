// Copyright (c) 2013, Yuri Kobets (tordex)
// Copyright (c) 2020-2022 Primate Labs Inc.
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

#include "litehtml/css/css_selector.h"

#include <assert.h>
#include <gtest/gtest.h>

#include "litehtml/css/css_parser.h"
#include "litehtml/css/css_tokenizer.h"

using namespace litehtml;

TEST(CSSSelectorTest, ElementSelectorParse)
{
    CSSElementSelector selector;
    // https://www.w3schools.com/cssref/CSSSelectors.asp
    selector.parse(".class");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "class"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "class"));

    selector.parse(".class1.class2");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 2);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "class1"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[1].val.c_str(), "class2"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "class"));

    selector.parse("#id");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "id"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "id"));

    selector.parse("*");
    EXPECT_TRUE(!strcmp(selector.m_tag.c_str(), "*"));
    EXPECT_TRUE(selector.m_attrs.empty());

    selector.parse("element");
    EXPECT_TRUE(!strcmp(selector.m_tag.c_str(), "element"));
    EXPECT_TRUE(selector.m_attrs.empty());

    selector.parse("[attribute]");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), ""));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "attribute"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectExists);

    selector.parse("[attribute=value]");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "value"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "attribute"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectEqual);

    selector.parse("[attribute~=value]");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "value"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "attribute"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectContainStr);

    selector.parse("[attribute|=value]");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "value"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "attribute"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectStartStr);

    selector.parse("[attribute^=value]");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "value"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "attribute"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectStartStr);

    selector.parse("[attribute$=value]");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "value"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "attribute"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectEndStr);

    selector.parse("[attribute*=value]");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "value"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "attribute"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectContainStr);

    selector.parse(":active");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "active"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse("::after");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "after"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo-el"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoElement);

    selector.parse("::before");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "before"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo-el"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoElement);

    selector.parse(":checked");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "checked"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":default");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "default"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":disabled");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "disabled"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":empty");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "empty"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":enabled");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "enabled"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":first-child");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "first-child"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse("::first-letter");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "first-letter"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo-el"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoElement);

    selector.parse("::first-line");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "first-line"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo-el"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoElement);

    selector.parse(":first-of-type");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "first-of-type"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":focus");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "focus"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":hover");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "hover"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":in-range");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "in-range"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":indeterminate");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "indeterminate"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":invalid");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "invalid"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":lang(language)");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "lang(language)"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":last-child");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "last-child"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":last-of-type");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "last-of-type"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":link");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "link"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":not(selector)");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "not(selector)"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":nth-child(n)");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "nth-child(n)"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":nth-last-child(n)");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(
        !strcmp(selector.m_attrs[0].val.c_str(), "nth-last-child(n)"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":nth-last-of-type(n)");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(
        !strcmp(selector.m_attrs[0].val.c_str(), "nth-last-of-type(n)"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":nth-of-type(n)");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "nth-of-type(n)"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":only-of-type");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "only-of-type"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":only-child");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "only-child"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":optional");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "optional"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":out-of-range");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "out-of-range"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse("::placeholder");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "placeholder"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo-el"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoElement);

    selector.parse(":read-only");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "read-only"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":read-write");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "read-write"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":required");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "required"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":root");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "root"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse("::selection");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "selection"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo-el"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoElement);

    selector.parse(":target");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "target"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":valid");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "valid"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    selector.parse(":visited");
    EXPECT_TRUE(selector.m_tag.empty());
    EXPECT_TRUE(selector.m_attrs.size() == 1);
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].val.c_str(), "visited"));
    EXPECT_TRUE(!strcmp(selector.m_attrs[0].attribute.c_str(), "pseudo"));
    EXPECT_TRUE(selector.m_attrs[0].condition == kSelectPseudoClass);

    // other
    selector.parse("tag:psudo#anchor");
    EXPECT_TRUE(!strcmp(selector.m_tag.c_str(), "tag"));
    EXPECT_TRUE(selector.m_attrs.size() == 2);
}


// CSSSelectorTest.SelectorParse fails on several systems yet passes on
// others. Disable the test until we can determine the cause of the failures.

TEST(CSSSelectorTest, DISABLED_SelectorParse)
{
    CSSSelector selector(nullptr);
    // https://www.w3schools.com/cssref/CSSSelectors.asp
    assert(!selector.parse(""));
    EXPECT_TRUE(selector.parse("element"));
    EXPECT_TRUE(selector.m_combinator == kCombinatorDescendant);
    EXPECT_TRUE(!strcmp(selector.m_right.m_tag.c_str(), "element"));
    EXPECT_TRUE(selector.m_right.m_attrs.empty());
    EXPECT_TRUE(selector.m_left == nullptr);

    // assert(selector.parse("element,element"));
    // EXPECT_TRUE(selector.m_combinator == kCombinatorDescendant);
    // EXPECT_TRUE(selector.m_right.m_tag.c_str(), "element");
    // EXPECT_TRUE(selector.m_right.m_attrs.empty());

    EXPECT_TRUE(selector.parse(".class1 .class2"));
    EXPECT_TRUE(selector.m_combinator == kCombinatorDescendant);
    EXPECT_TRUE(selector.m_right.m_tag.empty());
    EXPECT_TRUE(selector.m_right.m_attrs.size() == 1);
    EXPECT_TRUE(selector.m_left->m_right.m_attrs.size() == 1);

    assert(selector.parse("element element"));
    EXPECT_TRUE(selector.m_combinator == kCombinatorDescendant);
    EXPECT_TRUE(!strcmp(selector.m_right.m_tag.c_str(), "element"));
    EXPECT_TRUE(selector.m_right.m_attrs.empty());
    EXPECT_TRUE(!strcmp(selector.m_left->m_right.m_tag.c_str(), "element"));

    assert(selector.parse("element>element"));
    EXPECT_TRUE(selector.m_combinator == kCombinatorChild);
    EXPECT_TRUE(!strcmp(selector.m_right.m_tag.c_str(), "element"));
    EXPECT_TRUE(selector.m_right.m_attrs.empty());
    EXPECT_TRUE(!strcmp(selector.m_left->m_right.m_tag.c_str(), "element"));

    assert(selector.parse("element+element"));
    EXPECT_TRUE(selector.m_combinator == kCombinatorAdjacentSibling);
    EXPECT_TRUE(!strcmp(selector.m_right.m_tag.c_str(), "element"));
    EXPECT_TRUE(selector.m_right.m_attrs.empty());
    EXPECT_TRUE(!strcmp(selector.m_left->m_right.m_tag.c_str(), "element"));

    assert(selector.parse("element1~element2"));
    EXPECT_TRUE(selector.m_combinator == kCombinatorGeneralSibling);
    EXPECT_TRUE(!strcmp(selector.m_right.m_tag.c_str(), "element2"));
    EXPECT_TRUE(selector.m_right.m_attrs.empty());
    EXPECT_TRUE(!strcmp(selector.m_left->m_right.m_tag.c_str(), "element1"));
}
