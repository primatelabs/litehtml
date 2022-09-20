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

#include "litehtml/css/css_selector.h"

#include "litehtml/debug/json.h"
#include "litehtml/document.h"
#include "litehtml/html.h"

namespace litehtml {

void CSSElementSelector::parse(const tstring& txt)
{
    tstring::size_type el_end = txt.find_first_of(_t(".#[:"));
    m_tag = txt.substr(0, el_end);
    lcase(m_tag);
    m_attrs.clear();
    while (el_end != tstring::npos) {
        if (txt[el_end] == _t('.')) {
            CSSAttributeSelector attribute;

            tstring::size_type pos = txt.find_first_of(_t(".#[:"), el_end + 1);
            attribute.val = txt.substr(el_end + 1, pos - el_end - 1);
            split_string(attribute.val, attribute.class_val, _t(" "));
            attribute.condition = kSelectEqual;
            attribute.attribute = _t("class");
            m_attrs.push_back(attribute);
            el_end = pos;
        } else if (txt[el_end] == _t(':')) {
            CSSAttributeSelector attribute;

            if (txt[el_end + 1] == _t(':')) {
                tstring::size_type pos = txt.find_first_of(_t(".#[:"), el_end + 2);
                attribute.val = txt.substr(el_end + 2, pos - el_end - 2);
                attribute.condition = kSelectPseudoElement;
                lcase(attribute.val);
                attribute.attribute = _t("pseudo-el");
                m_attrs.push_back(attribute);
                el_end = pos;
            } else {
                tstring::size_type pos =
                    txt.find_first_of(_t(".#[:("), el_end + 1);
                if (pos != tstring::npos && txt.at(pos) == _t('(')) {
                    pos = find_close_bracket(txt, pos);
                    if (pos != tstring::npos) {
                        pos++;
                    } else {
                        int iii = 0;
                        iii++;
                    }
                }
                if (pos != tstring::npos) {
                    attribute.val = txt.substr(el_end + 1, pos - el_end - 1);
                } else {
                    attribute.val = txt.substr(el_end + 1);
                }
                lcase(attribute.val);
                if (attribute.val == _t("after") ||
                    attribute.val == _t("before")) {
                    attribute.condition = kSelectPseudoElement;
                } else {
                    attribute.condition = kSelectPseudoClass;
                }
                attribute.attribute = _t("pseudo");
                m_attrs.push_back(attribute);
                el_end = pos;
            }
        } else if (txt[el_end] == _t('#')) {
            CSSAttributeSelector attribute;

            tstring::size_type pos = txt.find_first_of(_t(".#[:"), el_end + 1);
            attribute.val = txt.substr(el_end + 1, pos - el_end - 1);
            attribute.condition = kSelectEqual;
            attribute.attribute = _t("id");
            m_attrs.push_back(attribute);
            el_end = pos;
        } else if (txt[el_end] == _t('[')) {
            CSSAttributeSelector attribute;

            tstring::size_type pos = txt.find_first_of(_t("]~=|$*^"), el_end + 1);
            tstring attr = txt.substr(el_end + 1, pos - el_end - 1);
            trim(attr);
            lcase(attr);
            if (pos != tstring::npos) {
                if (txt[pos] == _t(']')) {
                    attribute.condition = kSelectExists;
                } else if (txt[pos] == _t('=')) {
                    attribute.condition = kSelectEqual;
                    pos++;
                } else if (txt.substr(pos, 2) == _t("~=")) {
                    attribute.condition = kSelectContainStr;
                    pos += 2;
                } else if (txt.substr(pos, 2) == _t("|=")) {
                    attribute.condition = kSelectStartStr;
                    pos += 2;
                } else if (txt.substr(pos, 2) == _t("^=")) {
                    attribute.condition = kSelectStartStr;
                    pos += 2;
                } else if (txt.substr(pos, 2) == _t("$=")) {
                    attribute.condition = kSelectEndStr;
                    pos += 2;
                } else if (txt.substr(pos, 2) == _t("*=")) {
                    attribute.condition = kSelectContainStr;
                    pos += 2;
                } else {
                    attribute.condition = kSelectExists;
                    pos += 1;
                }
                pos = txt.find_first_not_of(_t(" \t"), pos);
                if (pos != tstring::npos) {
                    if (txt[pos] == _t('"')) {
                        tstring::size_type pos2 =
                            txt.find_first_of(_t("\""), pos + 1);
                        attribute.val = txt.substr(pos + 1,
                            pos2 == tstring::npos ? pos2 : (pos2 - pos - 1));
                        pos = pos2 == tstring::npos ? pos2 : (pos2 + 1);
                    } else if (txt[pos] == _t(']')) {
                        pos++;
                    } else {
                        tstring::size_type pos2 =
                            txt.find_first_of(_t("]"), pos + 1);
                        attribute.val = txt.substr(pos,
                            pos2 == tstring::npos ? pos2 : (pos2 - pos));
                        trim(attribute.val);
                        pos = pos2 == tstring::npos ? pos2 : (pos2 + 1);
                    }
                }
            } else {
                attribute.condition = kSelectExists;
            }
            attribute.attribute = attr;
            m_attrs.push_back(attribute);
            el_end = pos;
        } else {
            el_end++;
        }
        el_end = txt.find_first_of(_t(".#[:"), el_end);
    }
}


bool CSSSelector::parse(const tstring& text)
{
    if (text.empty()) {
        return false;
    }
    string_vector tokens;
    split_string(text, tokens, _t(""), _t(" \t>+~"), _t("(["));

    if (tokens.empty()) {
        return false;
    }

    tstring left;
    tstring right = tokens.back();
    tchar_t combinator = 0;

    tokens.pop_back();
    while (!tokens.empty() &&
           (tokens.back() == _t(" ") || tokens.back() == _t("\t") ||
               tokens.back() == _t("+") || tokens.back() == _t("~") ||
               tokens.back() == _t(">"))) {
        if (combinator == _t(' ') || combinator == 0) {
            combinator = tokens.back()[0];
        }
        tokens.pop_back();
    }

    for (string_vector::const_iterator i = tokens.begin(); i != tokens.end(); i++) {
        left += *i;
    }

    trim(left);
    trim(right);

    if (right.empty()) {
        return false;
    }

    m_right.parse(right);

    switch (combinator) {
        case _t('>'):
            m_combinator = kCombinatorChild;
            break;
        case _t('+'):
            m_combinator = kCombinatorAdjacentSibling;
            break;
        case _t('~'):
            m_combinator = kCombinatorGeneralSibling;
            break;
        default:
            m_combinator = kCombinatorDescendant;
            break;
    }

    m_left = nullptr;

    if (!left.empty()) {
        m_left = std::make_shared<CSSSelector>(MediaQueryList::ptr(nullptr));
        if (!m_left->parse(left)) {
            return false;
        }
    }

    return true;
}

void CSSSelector::calc_specificity()
{
    if (!m_right.m_tag.empty() && m_right.m_tag != _t("*")) {
        m_specificity.d = 1;
    }
    for (CSSAttributeSelector::vector::iterator i = m_right.m_attrs.begin();
         i != m_right.m_attrs.end();
         i++) {
        if (i->attribute == _t("id")) {
            m_specificity.b++;
        } else {
            if (i->attribute == _t("class")) {
                m_specificity.c += (int)i->class_val.size();
            } else {
                m_specificity.c++;
            }
        }
    }
    if (m_left) {
        m_left->calc_specificity();
        m_specificity += m_left->m_specificity;
    }
}

void CSSSelector::add_media_to_doc(Document* doc) const
{
    if (media_query_list_ && doc) {
        doc->add_media_list(media_query_list_);
    }
}

#if defined(ENABLE_JSON)

nlohmann::json CSSSelector::json() const
{
    nlohmann::json result = nlohmann::json{
        {"specificity", m_specificity.json()},
        {"right", m_right.json()},
        {"order", m_order},

    };

    if (m_style) {
        result["style"] = m_style->json();
    }

#if 0
    CSSSelectorSpecificity m_specificity;
    CSSElementSelector m_right;
    CSSSelector::ptr m_left;
    CSSCombinator m_combinator;
    CSSStyle::ptr m_style;
    int m_order;
    MediaQueryList::ptr media_query_list_;
#endif
    //return nlohmann::json::object();

    return result;
}

#endif

} // namespace litehtml
