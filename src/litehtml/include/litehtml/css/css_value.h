// Copyright (c) 2013, Yuri Kobets (tordex)
// Copyright (c) 2020-2021 Primate Labs Inc.
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

#ifndef LITEHTML_CSS_CSS_VALUE_H__
#define LITEHTML_CSS_CSS_VALUE_H__

#include "litehtml/css/css_length.h"
#include "litehtml/debug/json.h"
#include "litehtml/string.h"
#include "litehtml/web_color.h"

namespace litehtml {

enum CSSProperty : int;

using CSSKeyword = int;

enum CSSValueType : int {
    kCSSValueString,
    kCSSValueColor,
    kCSSValueKeyword,
    kCSSValueLength,
};

class CSSValue {
protected:
    CSSValueType type_ = kCSSValueString;

    tstring value_;

    bool important_ = false;

public:
    CSSValue() = delete;

    // FIXME: Legacy interface.
    CSSValue(const tstring& value, bool important);

    CSSValue(CSSValueType type, const tstring& value, bool important);

    CSSValueType type() const
    {
        return type_;
    }

    bool is_string() const
    {
        return type_ == kCSSValueString;
    }

    bool is_color() const
    {
        return type_ == kCSSValueColor;
    }

    bool is_keyword() const
    {
        return type_ == kCSSValueKeyword;
    }

    bool is_length() const
    {
        return type_ == kCSSValueLength;
    }

    const tstring& string() const
    {
        return value_;
    }

    void string(tstring& value)
    {
        value_ = value;
    }

    bool important() const
    {
        return important_;
    }

    void important(bool important)
    {
        important_ = important;
    }

    static CSSValue* factory(CSSProperty property, const tstring& value, bool important);

#if defined(ENABLE_JSON)
    nlohmann::json json() const;
#endif
};

class CSSColorValue : public CSSValue {
protected:
    WebColor color_;

public:
    CSSColorValue(WebColor color, const tstring& value, bool important);

    WebColor color() const
    {
        return color_;
    }
};

class CSSKeywordValue : public CSSValue {
protected:
  CSSKeyword keyword_;

public:
    CSSKeywordValue(CSSKeyword keyword, const tstring& value, bool important);

    CSSKeyword keyword() const
    {
        return keyword_;
    }
};

class CSSLengthValue : public CSSValue {
protected:
  CSSLength length_;

public:
    CSSLengthValue(const CSSLength& length, const tstring& value, bool important);

    const CSSLength& length() const
    {
        return length_;
    }
};

} // namespace litehtml

#endif // LITEHTML_CSS_CSS_VALUE_H__
