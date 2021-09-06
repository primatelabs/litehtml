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

#include "litehtml/types.h"

namespace litehtml {

enum css_token_type {
    kCSSTokenNone,
    kCSSTokenIdent,
    kCSSTokenFunction,
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
    kCSSTokenEOF,
};

class css_token {
protected:
    css_token_type type_;

public:
    css_token();

	explicit css_token(css_token_type type);

    css_token_type type() const {
        return type_;
    }
};

} // namespace litehtml

#endif // LITEHTML_CSS_TOKEN_H__
