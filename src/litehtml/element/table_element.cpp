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

#include "litehtml/element/table_element.h"

#include <algorithm>

#include "litehtml/document.h"
#include "litehtml/html.h"
#include "litehtml/iterators.h"

namespace litehtml {

TableElement::TableElement(Document* doc)
: HTMLElement(doc)
{
    m_border_spacing_x = 0;
    m_border_spacing_y = 0;
    m_border_collapse = border_collapse_separate;
}


TableElement::~TableElement()
{
}

bool TableElement::append_child(Element* element)
{
    if (!element)
        return false;
    if (!t_strcmp(element->get_tagName(), _t("tbody")) ||
        !t_strcmp(element->get_tagName(), _t("thead")) ||
        !t_strcmp(element->get_tagName(), _t("tfoot"))) {
        return HTMLElement::append_child(element);
    }
    return false;
}

void TableElement::parse_styles(bool is_reparse)
{
    HTMLElement::parse_styles(is_reparse);

    m_border_collapse = (border_collapse)value_index(
        get_style_property(kCSSPropertyBorderCollapse),
        BORDER_COLLAPSE_STRINGS,
        border_collapse_separate);

    if (m_border_collapse == border_collapse_separate) {
        m_css_border_spacing_x.parse_length_string(
            get_style_property(kCSSPropertyLitehtmlBorderSpacingX));
        m_css_border_spacing_y.parse_length_string(
            get_style_property(kCSSPropertyLitehtmlBorderSpacingY));

        int fntsz = get_font_size();
        Document* doc = get_document();
        m_border_spacing_x = doc->cvt_units(m_css_border_spacing_x, fntsz);
        m_border_spacing_y = doc->cvt_units(m_css_border_spacing_y, fntsz);
    } else {
        m_border_spacing_x = 0;
        m_border_spacing_y = 0;
        m_padding.bottom = 0;
        m_padding.top = 0;
        m_padding.left = 0;
        m_padding.right = 0;
        m_css_padding.bottom.set_value(0, kCSSUnitsPx);
        m_css_padding.top.set_value(0, kCSSUnitsPx);
        m_css_padding.left.set_value(0, kCSSUnitsPx);
        m_css_padding.right.set_value(0, kCSSUnitsPx);
    }
}

void TableElement::parse_attributes()
{
    const tchar_t* str = get_attr(_t("width"));
    if (str) {
        m_style.add_property(kCSSPropertyWidth, str, URL(), false);
    }

    str = get_attr(_t("align"));
    if (str) {
        int align = value_index(str, _t("left;center;right"));
        switch (align) {
            case 1:
                m_style.add_property(kCSSPropertyMarginLeft, _t("auto"), URL(), false);
                m_style.add_property(kCSSPropertyMarginRight, _t("auto"), URL(), false);
                break;
            case 2:
                m_style.add_property(kCSSPropertyMarginLeft, _t("auto"), URL(), false);
                m_style.add_property(kCSSPropertyMarginRight, _t("0"), URL(), false);
                break;
        }
    }

    str = get_attr(_t("cellspacing"));
    if (str) {
        tstring val = str;
        val += _t(" ");
        val += str;
        m_style.add_property(kCSSPropertyBorderSpacing, val.c_str(), URL(), false);
    }

    str = get_attr(_t("border"));
    if (str) {
        m_style.add_property(kCSSPropertyBorderWidth, str, URL(), false);
    }

    str = get_attr(_t("bgcolor"));
    if (str) {
        m_style.add_property(kCSSPropertyBackgroundColor, str, URL(), false);
    }

    HTMLElement::parse_attributes();
}

} // namespace litehtml
