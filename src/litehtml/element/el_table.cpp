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

#include "litehtml/element/el_table.h"

#include <algorithm>

#include "litehtml/document.h"
#include "litehtml/html.h"
#include "litehtml/iterators.h"

namespace litehtml {

el_table::el_table(const std::shared_ptr<document>& doc)
: html_tag(doc)
{
    m_border_spacing_x = 0;
    m_border_spacing_y = 0;
    m_border_collapse = border_collapse_separate;
}


el_table::~el_table()
{
}

bool el_table::appendChild(const element::ptr& el)
{
    if (!el)
        return false;
    if (!t_strcmp(el->get_tagName(), _t("tbody")) ||
        !t_strcmp(el->get_tagName(), _t("thead")) ||
        !t_strcmp(el->get_tagName(), _t("tfoot"))) {
        return html_tag::appendChild(el);
    }
    return false;
}

void el_table::parse_styles(bool is_reparse)
{
    html_tag::parse_styles(is_reparse);

    m_border_collapse = (border_collapse)value_index(
        get_style_property(_t("border-collapse"), true, _t("separate")),
        border_collapse_strings,
        border_collapse_separate);

    if (m_border_collapse == border_collapse_separate) {
        m_css_border_spacing_x.fromString(
            get_style_property(_t("-litehtml-border-spacing-x"), true, _t("0px")));
        m_css_border_spacing_y.fromString(
            get_style_property(_t("-litehtml-border-spacing-y"), true, _t("0px")));

        int fntsz = get_font_size();
        document::ptr doc = get_document();
        m_border_spacing_x = doc->cvt_units(m_css_border_spacing_x, fntsz);
        m_border_spacing_y = doc->cvt_units(m_css_border_spacing_y, fntsz);
    } else {
        m_border_spacing_x = 0;
        m_border_spacing_y = 0;
        m_padding.bottom = 0;
        m_padding.top = 0;
        m_padding.left = 0;
        m_padding.right = 0;
        m_css_padding.bottom.set_value(0, css_units_px);
        m_css_padding.top.set_value(0, css_units_px);
        m_css_padding.left.set_value(0, css_units_px);
        m_css_padding.right.set_value(0, css_units_px);
    }
}

void el_table::parse_attributes()
{
    const tchar_t* str = get_attr(_t("width"));
    if (str) {
        m_style.add_property(_t("width"), str, 0, false);
    }

    str = get_attr(_t("align"));
    if (str) {
        int align = value_index(str, _t("left;center;right"));
        switch (align) {
            case 1:
                m_style.add_property(_t("margin-left"), _t("auto"), 0, false);
                m_style.add_property(_t("margin-right"), _t("auto"), 0, false);
                break;
            case 2:
                m_style.add_property(_t("margin-left"), _t("auto"), 0, false);
                m_style.add_property(_t("margin-right"), _t("0"), 0, false);
                break;
        }
    }

    str = get_attr(_t("cellspacing"));
    if (str) {
        tstring val = str;
        val += _t(" ");
        val += str;
        m_style.add_property(_t("border-spacing"), val.c_str(), 0, false);
    }

    str = get_attr(_t("border"));
    if (str) {
        m_style.add_property(_t("border-width"), str, 0, false);
    }

    str = get_attr(_t("bgcolor"));
    if (str) {
        m_style.add_property(_t("background-color"), str, 0, false);
    }

    html_tag::parse_attributes();
}

} // namespace litehtml
