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
#include "litehtml/css/css_style.h"
#include "litehtml/debug/json.h"
#include "litehtml/url.h"

namespace litehtml {

class CSSStylesheet {
    CSSSelector::vector selectors_;

public:
    std::vector<CSSRule*> rules_;

public:
    CSSStylesheet() = default;

    ~CSSStylesheet() = default;

    const CSSSelector::vector& selectors() const
    {
        return selectors_;
    }

    void clear()
    {
        selectors_.clear();
    }

    void parse(const std::string& str,
        const URL& url,
        const Document* doc,
        const MediaQueryList::ptr& media);

    void sort_selectors();

    static void parse_css_url(const std::string& str, std::string& url);

public:
    void add_selector(CSSSelector::ptr selector)
    {
        selector->m_order = selectors_.size();
        selectors_.push_back(selector);
    }

#if defined(ENABLE_JSON)
    nlohmann::json json() const;
#endif // ENABLE_JSON
};

} // namespace litehtml

#endif // LITEHTML_CSS_STYLESHEET_H__
