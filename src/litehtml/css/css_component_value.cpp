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

#include "litehtml/css/css_component_value.h"

#include "litehtml/css/css_block.h"
#include "litehtml/css/css_function.h"
#include "litehtml/css/css_token.h"

namespace litehtml {

CSSComponentValue::CSSComponentValue(CSSTokenType type)
: type_(type)
{
}

CSSComponentValue::CSSComponentValue(CSSBlock* block)
: type_(kCSSTokenBlock)
, block_(block)
{
}

CSSComponentValue::CSSComponentValue(CSSFunction* function)
: type_(kCSSTokenFunction)
, function_(function)
{
}

CSSComponentValue::CSSComponentValue(CSSToken* token)
: type_(token->type())
, token_(token)
{
}

#if defined(ENABLE_JSON)

nlohmann::json CSSComponentValue::json() const
{
    nlohmann::json result{
        {"type", css_token_type_string(type_)}
    };

    switch (type_) {
        case kCSSTokenBlock:
            assert(block_);
            result["block"] = block_->json();
            break;

        case kCSSTokenFunction:
            assert(function_);
            result["function"] = function_->json();
            break;

        default:
            if (token_) {
                result["token"] = token_->json();
            }
            break;
    }

    return result;
}

#endif

} // namespace litehtml
