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

void CSSElementSelector::parse(const std::string& txt)
{
    std::string::size_type el_end = txt.find_first_of(".#[:");
    m_tag = txt.substr(0, el_end);
    lcase(m_tag);
    m_attrs.clear();
    while (el_end != std::string::npos) {
        if (txt[el_end] == '.') {
            CSSAttributeSelector attribute;

            std::string::size_type pos = txt.find_first_of(".#[:", el_end + 1);
            attribute.val = txt.substr(el_end + 1, pos - el_end - 1);
            split_string(attribute.val, attribute.class_val, " ");
            attribute.condition = kSelectEqual;
            attribute.attribute = "class";
            m_attrs.push_back(attribute);
            el_end = pos;
        } else if (txt[el_end] == ':') {
            CSSAttributeSelector attribute;

            if (txt[el_end + 1] == ':') {
                std::string::size_type pos = txt.find_first_of(".#[:", el_end + 2);
                attribute.val = txt.substr(el_end + 2, pos - el_end - 2);
                attribute.condition = kSelectPseudoElement;
                lcase(attribute.val);
                attribute.attribute = "pseudo-el";
                m_attrs.push_back(attribute);
                el_end = pos;
            } else {
                std::string::size_type pos =
                    txt.find_first_of(".#[:(", el_end + 1);
                if (pos != std::string::npos && txt.at(pos) == '(') {
                    pos = find_close_bracket(txt, pos);
                    if (pos != std::string::npos) {
                        pos++;
                    } else {
                        // FIXME: What do we do here?
                    }
                }
                if (pos != std::string::npos) {
                    attribute.val = txt.substr(el_end + 1, pos - el_end - 1);
                } else {
                    attribute.val = txt.substr(el_end + 1);
                }
                lcase(attribute.val);
                if (attribute.val == "after" ||
                    attribute.val == "before") {
                    attribute.condition = kSelectPseudoElement;
                } else {
                    attribute.condition = kSelectPseudoClass;
                }
                attribute.attribute = "pseudo";
                m_attrs.push_back(attribute);
                el_end = pos;
            }
        } else if (txt[el_end] == '#') {
            CSSAttributeSelector attribute;

            std::string::size_type pos = txt.find_first_of(".#[:", el_end + 1);
            attribute.val = txt.substr(el_end + 1, pos - el_end - 1);
            attribute.condition = kSelectEqual;
            attribute.attribute = "id";
            m_attrs.push_back(attribute);
            el_end = pos;
        } else if (txt[el_end] == '[') {
            CSSAttributeSelector attribute;

            std::string::size_type pos = txt.find_first_of("]~=|$*^", el_end + 1);
            std::string attr = txt.substr(el_end + 1, pos - el_end - 1);
            trim(attr);
            lcase(attr);
            if (pos != std::string::npos) {
                if (txt[pos] == ']') {
                    attribute.condition = kSelectExists;
                } else if (txt[pos] == '=') {
                    attribute.condition = kSelectEqual;
                    pos++;
                } else if (txt.substr(pos, 2) == "~=") {
                    attribute.condition = kSelectContainStr;
                    pos += 2;
                } else if (txt.substr(pos, 2) == "|=") {
                    attribute.condition = kSelectStartStr;
                    pos += 2;
                } else if (txt.substr(pos, 2) == "^=") {
                    attribute.condition = kSelectStartStr;
                    pos += 2;
                } else if (txt.substr(pos, 2) == "$=") {
                    attribute.condition = kSelectEndStr;
                    pos += 2;
                } else if (txt.substr(pos, 2) == "*=") {
                    attribute.condition = kSelectContainStr;
                    pos += 2;
                } else {
                    attribute.condition = kSelectExists;
                    pos += 1;
                }
                pos = txt.find_first_not_of(" \t", pos);
                if (pos != std::string::npos) {
                    if (txt[pos] == '"') {
                        std::string::size_type pos2 =
                            txt.find_first_of("\"", pos + 1);
                        attribute.val = txt.substr(pos + 1,
                            pos2 == std::string::npos ? pos2 : (pos2 - pos - 1));
                        pos = pos2 == std::string::npos ? pos2 : (pos2 + 1);
                    } else if (txt[pos] == ']') {
                        pos++;
                    } else {
                        std::string::size_type pos2 =
                            txt.find_first_of("]", pos + 1);
                        attribute.val = txt.substr(pos,
                            pos2 == std::string::npos ? pos2 : (pos2 - pos));
                        trim(attribute.val);
                        pos = pos2 == std::string::npos ? pos2 : (pos2 + 1);
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
        el_end = txt.find_first_of(".#[:", el_end);
    }
}


bool CSSSelector::parse(const std::string& text)
{
    if (text.empty()) {
        return false;
    }
    string_vector tokens;
    split_string(text, tokens, "", " \t>+~", "([");

    if (tokens.empty()) {
        return false;
    }

    std::string left;
    std::string right = tokens.back();
    char combinator = 0;

    tokens.pop_back();
    while (!tokens.empty() &&
           (tokens.back() == " " || tokens.back() == "\t" ||
               tokens.back() == "+" || tokens.back() == "~" ||
               tokens.back() == ">")) {
        if (combinator == ' ' || combinator == 0) {
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
        case '>':
            m_combinator = kCombinatorChild;
            break;
        case '+':
            m_combinator = kCombinatorAdjacentSibling;
            break;
        case '~':
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
    if (!m_right.m_tag.empty() && m_right.m_tag != "*") {
        m_specificity.d = 1;
    }
    for (CSSAttributeSelector::vector::iterator i = m_right.m_attrs.begin();
         i != m_right.m_attrs.end();
         i++) {
        if (i->attribute == "id") {
            m_specificity.b++;
        } else {
            if (i->attribute == "class") {
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
