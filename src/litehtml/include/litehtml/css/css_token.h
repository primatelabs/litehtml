// Copyright (C) 2020 Primate Labs Inc. All rights reserved.
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
//    * Neither the name of the copyright holder nor the names of its
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

#ifndef LITEHTML_CSS_TOKEN_H__
#define LITEHTML_CSS_TOKEN_H__

#include "litehtml/css/css_number.h"
#include "litehtml/debug/json.h"
#include "litehtml/types.h"

namespace litehtml {

enum CSSTokenType {
    kCSSTokenNone,
    kCSSTokenIdent,
    kCSSTokenAtKeyword,
    kCSSTokenHash,
    kCSSTokenString,
    kCSSTokenBadString,
    kCSSTokenURL,
    kCSSTokenBadURL,
    kCSSTokenDelim,
    kCSSTokenNumber,
    kCSSTokenPercentage,
    kCSSTokenDimension,
    kCSSTokenWhitespace,
    kCSSTokenCDO,
    kCSSTokenCDC,
    kCSSTokenColon,
    kCSSTokenSemicolon,
    kCSSTokenComma,
    kCSSTokenOpenSquareBracket,
    kCSSTokenCloseSquareBracket,
    kCSSTokenOpenRoundBracket,
    kCSSTokenCloseRoundBracket,
    kCSSTokenOpenBrace,
    kCSSTokenCloseBrace,

    // Used to represent either a function token or a component value that
    // contains a function.
    kCSSTokenFunction,

    // Used to represent a component value that contains a block.
    kCSSTokenBlock,

    // Used to represent the end of a list of tokens or component values.
    kCSSTokenEOF,
};

std::string css_token_type_string(CSSTokenType type);

class CSSToken {
protected:
    CSSTokenType type_;

    std::string value_;

    CSSNumber numeric_value_;

public:
    CSSToken();

    explicit CSSToken(CSSTokenType type);

    CSSToken(CSSTokenType type, char value);

    CSSToken(CSSTokenType type, const std::string& value);

    CSSToken(CSSTokenType type, const CSSNumber& numeric_value);

    CSSTokenType type() const
    {
        return type_;
    }

    const std::string& value() const
    {
        return value_;
    }

    const CSSNumber& numeric_value() const
    {
        return numeric_value_;
    }

#if defined(ENABLE_JSON)
    nlohmann::json json() const
    {
        return nlohmann::json{
            {"type", css_token_type_string(type_)},
            {"value", value_},
            {"numeric_value_", numeric_value_.json()},
        };
    }
#endif // ENABLE_JSON
};

} // namespace litehtml

#endif // LITEHTML_CSS_TOKEN_H__
