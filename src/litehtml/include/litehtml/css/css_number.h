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

#ifndef LITEHTML_CSS_NUMBER_H__
#define LITEHTML_CSS_NUMBER_H__

#include "litehtml/debug/json.h"
#include "litehtml/types.h"

namespace litehtml {

enum CSSNumberValueType {
    kCSSIntegerValue,
    kCSSNumberValue,
};

class CSSNumber {
    CSSNumberValueType type_;
    double value_;

public:
    CSSNumber()
    : type_(kCSSIntegerValue)
    , value_(0.0)
    {
    }

    CSSNumber(CSSNumberValueType type, double value)
    : type_(type)
    , value_(value)
    {
    }

    CSSNumberValueType type() const
    {
        return type_;
    }

    double value() const
    {
        return value_;
    }

#if defined(ENABLE_JSON)
    nlohmann::json json() const
    {
        return nlohmann::json{
            {"type", type_},
            {"value", value_},
        };
    }
#endif // ENABLE_JSON
};

} // namespace litehtml

#endif // LITEHTML_CSS_NUMBER_H__
