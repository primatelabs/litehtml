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
//    * Neither the name of the copyright holders nor the names of their
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

#ifndef LITEHTML_CSS_STYLESHEET_H__
#define LITEHTML_CSS_STYLESHEET_H__

#include <vector>

#include "litehtml/css/css_rule.h"
#include "litehtml/css/css_selector.h"
#include "litehtml/debug/json.h"
#include "litehtml/style.h"

namespace litehtml {

class css_stylesheet {
    css_selector::vector m_selectors;

public:
    std::vector<css_rule*> rules_;

public:
    css_stylesheet()
    {
    }

    ~css_stylesheet()
    {
    }

    const css_selector::vector& selectors() const
    {
        return m_selectors;
    }

    void clear()
    {
        m_selectors.clear();
    }

    void parse_stylesheet(const tchar_t* str,
        const tchar_t* baseurl,
        const std::shared_ptr<document>& doc,
        const media_query_list::ptr& media);

    void sort_selectors();

    static void parse_css_url(const tstring& str, tstring& url);

public:
    void parse_atrule(const tstring& text,
        const tchar_t* baseurl,
        const std::shared_ptr<document>& doc,
        const media_query_list::ptr& media);

    void add_selector(css_selector::ptr selector)
    {
        selector->m_order = (int)m_selectors.size();
        m_selectors.push_back(selector);
    }

    bool parse_selectors(const tstring& txt,
        const litehtml::style::ptr& styles,
        const media_query_list::ptr& media);

#if defined(ENABLE_JSON)
    nlohmann::json json() const
    {
        return nlohmann::json{{"rules", json_vector(rules_)}};
    }
#endif // ENABLE_JSON
};

} // namespace litehtml

#endif // LITEHTML_CSS_STYLESHEET_H__
