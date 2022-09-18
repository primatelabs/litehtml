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

#include "litehtml/element/before_after_element.h"

#include "litehtml/element/image_element.h"
#include "litehtml/element/whitespace_element.h"
#include "litehtml/element/text_element.h"
#include "litehtml/html.h"

namespace litehtml {

BeforeAfterBaseElement::BeforeAfterBaseElement(Document* doc, bool before)
: HTMLElement(doc)
{
    if (before) {
        set_tagName(_t("::before"));
    } else {
        set_tagName(_t("::after"));
    }
}

BeforeAfterBaseElement::~BeforeAfterBaseElement()
{
}

void BeforeAfterBaseElement::add_style(const CSSStyle& st)
{
    HTMLElement::add_style(st);

    tstring content = get_style_property(kCSSPropertyContent);
    if (!content.empty()) {
        int idx = value_index(content.c_str(), CONTENT_PROPERTY_STRING);
        if (idx < 0) {
            tstring fnc;
            tstring::size_type i = 0;
            while (i < content.length() && i != tstring::npos) {
                if (content.at(i) == _t('"')) {
                    fnc.clear();
                    i++;
                    tstring::size_type pos = content.find(_t('"'), i);
                    tstring txt;
                    if (pos == tstring::npos) {
                        txt = content.substr(i);
                        i = tstring::npos;
                    } else {
                        txt = content.substr(i, pos - i);
                        i = pos + 1;
                    }
                    add_text(txt);
                } else if (content.at(i) == _t('(')) {
                    i++;
                    trim(fnc);
                    lcase(fnc);
                    tstring::size_type pos = content.find(_t(')'), i);
                    tstring params;
                    if (pos == tstring::npos) {
                        params = content.substr(i);
                        i = tstring::npos;
                    } else {
                        params = content.substr(i, pos - i);
                        i = pos + 1;
                    }
                    add_function(fnc, params);
                    fnc.clear();
                } else {
                    fnc += content.at(i);
                    i++;
                }
            }
        }
    }
}

// TODO: Is this code similar enough to split_text_node() that we should
// consolidate the two functions?
void BeforeAfterBaseElement::add_text(const tstring& txt)
{
    tstring word;
    tstring esc;
    for (tstring::size_type i = 0; i < txt.length(); i++) {
        if ((txt.at(i) == _t(' ')) || (txt.at(i) == _t('\t')) ||
            (txt.at(i) == _t('\\') && !esc.empty())) {
            if (esc.empty()) {
                if (!word.empty()) {
                    Element::ptr el = new TextElement(get_document(), word.c_str());
                    append_child(el);
                    word.clear();
                }

                Element* element = new WhitespaceElement(get_document(), txt.substr(i, 1).c_str());
                append_child(element);
            } else {
                word += convert_escape(esc.c_str() + 1);
                esc.clear();
                if (txt.at(i) == _t('\\')) {
                    esc += txt.at(i);
                }
            }
        } else {
            if (!esc.empty() || txt.at(i) == _t('\\')) {
                esc += txt.at(i);
            } else {
                word += txt.at(i);
            }
        }
    }

    if (!esc.empty()) {
        word += convert_escape(esc.c_str() + 1);
    }
    if (!word.empty()) {
        Element::ptr el = new TextElement(get_document(), word.c_str());
        append_child(el);
        word.clear();
    }
}

void BeforeAfterBaseElement::add_function(const tstring& fnc, const tstring& params)
{
    int idx = value_index(fnc.c_str(), _t("attr;counter;url"));
    switch (idx) {
        // attr
        case 0: {
            tstring p_name = params;
            trim(p_name);
            lcase(p_name);
            Element::ptr el_parent = parent();
            if (el_parent) {
                const tchar_t* attr_value = el_parent->get_attr(p_name.c_str());
                if (attr_value) {
                    add_text(attr_value);
                }
            }
        } break;
        // counter
        case 1:
            break;
        // url
        case 2: {
            tstring p_url = params;
            trim(p_url);
            if (!p_url.empty()) {
                if (p_url.at(0) == _t('\'') || p_url.at(0) == _t('\"')) {
                    p_url.erase(0, 1);
                }
            }
            if (!p_url.empty()) {
                if (p_url.at(p_url.length() - 1) == _t('\'') ||
                    p_url.at(p_url.length() - 1) == _t('\"')) {
                    p_url.erase(p_url.length() - 1, 1);
                }
            }
            if (!p_url.empty()) {
                Element::ptr el = new ImageElement(get_document());
                el->set_attr(_t("src"), p_url.c_str());
                el->set_attr(_t("style"), _t("display:inline-block"));
                el->set_tagName(_t("img"));
                append_child(el);
                el->parse_attributes();
            }
        } break;
    }
}

tchar_t BeforeAfterBaseElement::convert_escape(const tchar_t* txt)
{
    tchar_t* sss = nullptr;
    return (tchar_t)t_strtol(txt, &sss, 16);
}

void BeforeAfterBaseElement::apply_stylesheet(const CSSStylesheet&)
{
}

BeforeElement::~BeforeElement()
{
}

AfterElement::~AfterElement()
{
}

} // namespace litehtml
