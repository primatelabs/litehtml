// Copyright (c) 2013, Yuri Kobets (tordex)
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

#include "litehtml/element/font_element.h"

namespace litehtml {

FontElement::FontElement(Document* doc)
: HTMLElement(doc)
{
}

FontElement::~FontElement()
{
}

void FontElement::parse_attributes()
{
    const tchar_t* str = get_attr(_t("color"));
    if (str) {
        m_style.add_property(kCSSPropertyColor, str, URL(), false);
    }

    str = get_attr(_t("face"));
    if (str) {
        m_style.add_property(_t("font-face"), str, URL(), false);
    }

    str = get_attr(_t("size"));
    if (str) {
        int sz = t_atoi(str);
        if (sz <= 1) {
            m_style.add_property(kCSSPropertyFontSize, _t("x-small"), URL(), false);
        } else if (sz >= 6) {
            m_style.add_property(kCSSPropertyFontSize, _t("xx-large"), URL(), false);
        } else {
            switch (sz) {
                case 2:
                    m_style.add_property(kCSSPropertyFontSize, _t("small"), URL(), false);
                    break;
                case 3:
                    m_style.add_property(kCSSPropertyFontSize, _t("medium"), URL(), false);
                    break;
                case 4:
                    m_style.add_property(kCSSPropertyFontSize, _t("large"), URL(), false);
                    break;
                case 5:
                    m_style.add_property(kCSSPropertyFontSize, _t("x-large"), URL(), false);
                    break;
            }
        }
    }

    HTMLElement::parse_attributes();
}

} // namespace litehtml
