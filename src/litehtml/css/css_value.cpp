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

#include "litehtml/css/css_value.h"

#include "litehtml/css/css_property.h"
#include "litehtml/logging.h"

namespace litehtml {

CSSValue::CSSValue(const tstring& value, bool important)
: type_(kCSSValueString)
, value_(value)
, important_(important)
{
}

CSSValue::CSSValue(CSSValueType type, const tstring& value, bool important)
: type_(type)
, value_(value)
, important_(important)
{
}

CSSValue* CSSValue::factory(CSSProperty property, const tstring& str, bool important)
{
    CSSValueType type = css_property_value_type(property);

    // LOG(INFO) << "CSSValue::factory";
    // LOG(INFO) << css_property_string(property) << " " << str << " " << important;

    switch (type) {
        case kCSSValueString: {
            return new CSSValue(kCSSValueString, str, important);
        }

        case kCSSValueColor: {
            Color color(str);
            return new CSSColorValue(color, str, important);
        }

        case kCSSValueKeyword: {
            auto keywords = css_property_keywords(property);
            for (auto& keyword : keywords) {
                if (!t_strcasecmp(keyword.first.c_str(), str.c_str())) {
                    return new CSSKeywordValue(keyword.second, str, important);
                }
            }
            return new CSSKeywordValue(kCSSKeywordNone, str, important);
        }

        case kCSSValueLength: {
            CSSLength length;
            length.parse_length_string(str, css_property_keywords(property), 0);

            return new CSSLengthValue(length, str, important);
        }

        default:
            return nullptr;
    }
}

#if defined(ENABLE_JSON)

nlohmann::json CSSValue::json() const
{
    return nlohmann::json{
        {"value", value_},
        {"important", important_}
    };
}

#endif // ENABLE_JSON

CSSColorValue::CSSColorValue(Color color, const tstring& value, bool important)
: CSSValue(kCSSValueColor, value, important)
, color_(color)
{
}

CSSKeywordValue::CSSKeywordValue(CSSKeyword keyword, const tstring& value, bool important)
: CSSValue(kCSSValueKeyword, value, important)
, keyword_(keyword)
{
}

CSSLengthValue::CSSLengthValue(const CSSLength& length, const tstring& str, bool important)
: CSSValue(kCSSValueLength, str, important)
, length_(length)
{
}

} // namespace litehtml
