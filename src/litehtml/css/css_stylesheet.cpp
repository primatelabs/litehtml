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

#include "litehtml/css/css_stylesheet.h"

#include <algorithm>
#include <iostream>

#include "litehtml/css/css_parser.h"
#include "litehtml/document.h"
#include "litehtml/document_container.h"
#include "litehtml/html.h"

namespace litehtml {

void CSSStylesheet::parse(const std::string& str,
    const URL&,
    const Document*,
    const MediaQueryList::ptr&)
{
    CSSParser parser(str);
    parser.parse_stylesheet(this);
}

void CSSStylesheet::parse_css_url(const std::string& str, std::string& url)
{
    url = "";
    size_t pos1 = str.find('(');
    size_t pos2 = str.find(')');
    if (pos1 != std::string::npos && pos2 != std::string::npos) {
        url = str.substr(pos1 + 1, pos2 - pos1 - 1);
        if (url.length()) {
            if (url[0] == '\'' || url[0] == '"') {
                url.erase(0, 1);
            }
        }
        if (url.length()) {
            if (url[url.length() - 1] == '\'' ||
                url[url.length() - 1] == '"') {
                url.erase(url.length() - 1, 1);
            }
        }
    }
}

void CSSStylesheet::sort_selectors()
{
    std::sort(selectors_.begin(),
        selectors_.end(),
        [](const CSSSelector::ptr& v1, const CSSSelector::ptr& v2) {
            return (*v1) < (*v2);
        });
}

#if defined(ENABLE_JSON)

nlohmann::json CSSStylesheet::json() const
{
    nlohmann::json result;

    result["selectors"] = nlohmann::json::array();

    for (auto selector: selectors_) {
        result["selectors"].push_back(selector->json());
    }

    return result;
}

#endif // ENABLE_JSON

} // namespace litehtml
