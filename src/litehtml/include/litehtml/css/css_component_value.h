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

#ifndef LITEHTML_CSS_COMPONENT_VALUE_H__
#define LITEHTML_CSS_COMPONENT_VALUE_H__

#include <string>

#include "litehtml/css/css_token.h"
#include "litehtml/debug/json.h"

namespace litehtml {

class CSSBlock;
class CSSFunction;
class CSSToken;

class CSSComponentValue {
public:
    // Using CSSTokenType as the component value type makes the CSSParser
    // implementation much simpler.  In fact, the CSS Syntax Module Level 3
    // document effectively recommends this approach:
    //
    //   https://www.w3.org/TR/css-syntax-3/#parser-definitions

    CSSTokenType type_ = kCSSTokenNone;

    CSSBlock* block_ = nullptr;

    CSSFunction* function_ = nullptr;

    CSSToken* token_ = nullptr;

public:
    CSSComponentValue() = default;

    explicit CSSComponentValue(CSSTokenType type);

    explicit CSSComponentValue(CSSBlock* block);

    explicit CSSComponentValue(CSSFunction* function);

    explicit CSSComponentValue(CSSToken* token);

    ~CSSComponentValue() = default;

    const CSSBlock* block() const
    {
        return block_;
    }

    const CSSFunction* function() const
    {
        return function_;
    }

    const CSSToken* token() const
    {
        return token_;
    }

    CSSTokenType type() const
    {
        return type_;
    }

#if defined(ENABLE_JSON)
    nlohmann::json json() const;
#endif // ENABLE_JSON
};

} // namespace litehtml

#endif // LITEHTML_CSS_COMPONENT_VALUE_H__
