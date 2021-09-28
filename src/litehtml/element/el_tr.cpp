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

#include "litehtml/element/el_tr.h"

#include "litehtml/html.h"

namespace litehtml {

litehtml::el_tr::el_tr(const std::shared_ptr<litehtml::document>& doc)
: html_tag(doc)
{
}

litehtml::el_tr::~el_tr()
{
}

void litehtml::el_tr::parse_attributes()
{
    const tchar_t* str = get_attr(_t("align"));
    if (str) {
        m_style.add_property(_t("text-align"), str, 0, false);
    }
    str = get_attr(_t("valign"));
    if (str) {
        m_style.add_property(_t("vertical-align"), str, 0, false);
    }
    str = get_attr(_t("bgcolor"));
    if (str) {
        m_style.add_property(_t("background-color"), str, 0, false);
    }
    html_tag::parse_attributes();
}

void litehtml::el_tr::get_inline_boxes(position::vector& boxes)
{
    position pos;
    for (auto& el : m_children) {
        if (el->get_display() == display_table_cell) {
            pos.x = el->left() + el->margin_left();
            pos.y = el->top() - m_padding.top - m_borders.top;

            pos.width =
                el->right() - pos.x - el->margin_right() - el->margin_left();
            pos.height = el->height() + m_padding.top + m_padding.bottom +
                         m_borders.top + m_borders.bottom;

            boxes.push_back(pos);
        }
    }
}

} // namespace litehtml