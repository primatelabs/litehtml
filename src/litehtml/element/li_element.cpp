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

#include "litehtml/element/li_element.h"

#include "litehtml/document.h"

namespace litehtml {

LiElement::LiElement(Document* doc)
: HTMLElement(doc)
{
}

LiElement::~LiElement()
{
}

int LiElement::render(int x, int y, int max_width, bool second_pass)
{
    if (list_style_type_ >= kListStyleTypeArmenian && !m_index_initialized) {
        if (auto p = parent()) {
            char val[2] = {1, 0};
            for (int i = 0, n = (int)p->get_children_count(); i < n; ++i) {
                auto child = p->get_child(i);
                if (child == this) {
                    set_attr("list_index", val);
                    break;
                } else if (!strcmp(child->get_tagName(), "li"))
                    ++val[0];
            }
        }

        m_index_initialized = true;
    }

    return HTMLElement::render(x, y, max_width, second_pass);
}

} // namespace litehtml
