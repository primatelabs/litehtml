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

#ifndef LITEHTML_STYLE_H__
#define LITEHTML_STYLE_H__

#include <assert.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "litehtml/css/css_declaration.h"
#include "litehtml/css/css_property.h"
#include "litehtml/css/css_value.h"
#include "litehtml/debug/json.h"
#include "litehtml/url.h"

namespace litehtml {

class CSSStyle {
public:
    typedef std::shared_ptr<CSSStyle> ptr;
    typedef std::vector<CSSStyle::ptr> vector;

private:
    // FIXME: Consider using unique_ptr<CSSValue> instead of CSSValue* to
    // automate memory management and prevent memory leaks.  First, though, we
    // have to determine if any other classes store these CSSValue instances.

    std::unordered_map<CSSProperty, std::unique_ptr<CSSValue>> properties_;

public:
    CSSStyle();

    virtual ~CSSStyle();

    void add(const tstring& txt, const URL& baseurl)
    {
        parse(txt, baseurl);
    }

    // TODO: Old interface, remove
    void add_property(const tchar_t* str,
        const tchar_t* val,
        const URL& baseurl,
        bool important)
    {
        CSSProperty name = css_property_from_string(str);
        if (name == kCSSPropertyUnknown) {
            // LOG(WARNING) << "unrecognized CSS property " << str;
        }
        add_property(name, val, baseurl, important);
    }

    void add_property(CSSProperty name,
        const tchar_t* val,
        const URL& baseurl,
        bool important);

    void add_property(CSSDeclaration* declaration);

    const tchar_t* get_property(CSSProperty name) const
    {
        auto declaration = properties_.find(name);
        if (declaration != properties_.end()) {
            return declaration->second->string().c_str();
        }

        return nullptr;
    }

    const CSSValue* get_property_value(CSSProperty property) const
    {
        auto iterator = properties_.find(property);
        if (iterator != properties_.end()) {
            return iterator->second.get();
        }

        return nullptr;
    }

    void combine(const CSSStyle& other);

    void clear()
    {
        properties_.clear();
    }

#if defined(ENABLE_JSON)
    nlohmann::json json() const;
#endif

private:
    void parse_property(const tstring& txt, const URL& url);

    void parse(const tstring& txt, const URL& url);

    void parse_short_border(CSSProperty prefix, const tstring& val, bool important);

    void parse_short_background(const tstring& val,
        const URL& url,
        bool important);

    void parse_short_font(const tstring& val, bool important);

    // TODO: Old interface, remove
    void add_parsed_property(const tstring& str, const tstring& val, bool important)
    {
        CSSProperty name = css_property_from_string(str);
        if (name == kCSSPropertyUnknown) {
            assert(false);
        }
        add_parsed_property(name, val, important);
    }

    void add_parsed_property(CSSProperty name, const tstring& val, bool important);

    void remove_property(CSSProperty name, bool important);
};
} // namespace litehtml

#endif // LITEHTML_STYLE_H__