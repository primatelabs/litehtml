// Copyright (C) 2020-2021 Primate Labs Inc. All rights reserved.
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

#include "litehtml/css/css_regenerate.h"

#include <iostream>
#include <sstream>

#include "litehtml/css/css_function.h"

namespace litehtml {

namespace {

void regenerate_token(std::ostringstream& oss, const CSSToken* token)
{
    switch (token->type()) {
        case kCSSTokenAtKeyword:
            oss << "@" << token->value();
            break;
        case kCSSTokenHash:
            oss << "#" << token->value();
            break;
        case kCSSTokenString:
            // TODO: Escape string value
            oss << "\"" << token->value() << "\"";
            break;
        case kCSSTokenURL:
            oss << "url(" << token->value() << ")";
            break;
        case kCSSTokenNumber: {
            const CSSNumber& number = token->numeric_value();
            if (number.type() == kCSSIntegerValue) {
                oss << static_cast<int>(number.value());
            } else {
                oss << number.value();
            }
            break;
        }
        case kCSSTokenWhitespace:
            oss << " ";
            break;
        case kCSSTokenColon:
            oss << ":";
            break;
        case kCSSTokenSemicolon:
            oss << ";";
            break;
        case kCSSTokenComma:
            oss << ",";
            break;
        case kCSSTokenOpenSquareBracket:
            oss << "[";
            break;
        case kCSSTokenCloseSquareBracket:
            oss << "[";
            break;
        case kCSSTokenOpenRoundBracket:
            oss << "(";
            break;
        case kCSSTokenCloseRoundBracket:
            oss << ")";
            break;
        case kCSSTokenOpenBrace:
            oss << "{";
            break;
        case kCSSTokenCloseBrace:
            oss << "}";
            break;
        case kCSSTokenFunction: {
            oss << token->value() << "(";
            break;
        }
        default:
            oss << token->value();
            break;
    }
}

void regenerate_component_value(std::ostringstream& oss, const CSSComponentValue* value)
{
    switch (value->type()) {
        case kCSSTokenBlock:
            // TODO: Implement
            //assert(false);
            break;

        case kCSSTokenFunction: {
            const CSSFunction* function = value->function();

            oss << function->name_ << "(";
            for (const CSSComponentValue* value : function->values_) {
                regenerate_component_value(oss, value);
            }
            oss << ")";
            break;
        }

        default:
            regenerate_token(oss, value->token());
            break;
    }
}

} // namespace

String css_regenerate(const std::vector<const CSSComponentValue*>& values)
{
    std::ostringstream oss;

    for (const CSSComponentValue* value : values) {
        regenerate_component_value(oss, value);
    }

    return oss.str();
}

String css_regenerate(const std::vector<CSSComponentValue*>& values)
{
    std::ostringstream oss;

    for (CSSComponentValue* value : values) {
        regenerate_component_value(oss, value);
    }

    return oss.str();
}

String css_regenerate(const std::vector<const CSSToken*>& values)
{
    std::ostringstream oss;

    for (const CSSToken* value : values) {
        regenerate_token(oss, value);
    }

    return oss.str();
}

String css_regenerate(std::vector<CSSToken*>& values)
{
    std::ostringstream oss;

    for (CSSToken* value : values) {
        std::cout << value->type() << " " << css_token_type_string(value->type()) << " " << value->value() << std::endl;
        regenerate_token(oss, value);
    }

    return oss.str();
}

String css_regenerate(const std::vector<CSSToken>& values)
{
    std::ostringstream oss;

    for (const CSSToken& value : values) {
        regenerate_token(oss, &value);
    }

    return oss.str();
}

} // namespace litehtml
