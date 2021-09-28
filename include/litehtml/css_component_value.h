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

#include "litehtml/css_block.h"
#include "litehtml/css_function.h"
#include "litehtml/css_token.h"
#include "litehtml/debug/json.h"

namespace litehtml {

enum css_component_value_type {
    kCSSComponentValueNone,
    kCSSComponentValueBlock,
    kCSSComponentValueFunction,
    kCSSComponentValueToken,
};

std::string css_component_value_type_string(css_component_value_type type);

class css_component_value {
public:
    css_block block_;

    css_function function_;

    css_token token_;

    css_component_value_type type_ = kCSSComponentValueNone;

public:
    css_component_value() = default;

    ~css_component_value() = default;

#if defined(ENABLE_JSON)
    nlohmann::json json() const
    {
        return nlohmann::json{
            {"block", block_.json()},
            {"function", function_.json()},
            {"token", token_.json()},
            {"type", css_component_value_type_string(type_)},
        };
    }
#endif // ENABLE_JSON
};

} // namespace litehtml

#endif // LITEHTML_CSS_COMPONENT_VALUE_H__
