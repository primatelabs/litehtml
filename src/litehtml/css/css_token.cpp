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

#include "litehtml/css/css_token.h"

namespace litehtml {

#define TOKEN_CASE(t)  \
    case kCSSToken##t: \
        return #t;

std::string css_token_type_string(CSSTokenType type)
{
    switch (type) {
        TOKEN_CASE(None);
        TOKEN_CASE(Ident);
        TOKEN_CASE(AtKeyword);
        TOKEN_CASE(Hash);
        TOKEN_CASE(String);
        TOKEN_CASE(BadString);
        TOKEN_CASE(URL);
        TOKEN_CASE(BadURL);
        TOKEN_CASE(Delim);
        TOKEN_CASE(Number);
        TOKEN_CASE(Percentage);
        TOKEN_CASE(Dimension);
        TOKEN_CASE(Whitespace);
        TOKEN_CASE(CDO);
        TOKEN_CASE(CDC);
        TOKEN_CASE(Colon);
        TOKEN_CASE(Semicolon);
        TOKEN_CASE(Comma);
        TOKEN_CASE(OpenSquareBracket);
        TOKEN_CASE(CloseSquareBracket);
        TOKEN_CASE(OpenRoundBracket);
        TOKEN_CASE(CloseRoundBracket);
        TOKEN_CASE(OpenBrace);
        TOKEN_CASE(CloseBrace);
        TOKEN_CASE(Function);
        TOKEN_CASE(Block);
        TOKEN_CASE(EOF);
        default:
            return "Unknown";
    }
}

CSSToken::CSSToken()
: type_(kCSSTokenNone)
{
}

CSSToken::CSSToken(CSSTokenType type)
: type_(type)
{
}

CSSToken::CSSToken(CSSTokenType type, tchar_t value)
: type_(type)
, value_(1, value)
{

}

CSSToken::CSSToken(CSSTokenType type, const tstring& value)
: type_(type)
, value_(value)
{
}

CSSToken::CSSToken(CSSTokenType type, const CSSNumber& numeric_value)
: type_(type)
, numeric_value_(numeric_value)
{
}

} // namespace litehtml
