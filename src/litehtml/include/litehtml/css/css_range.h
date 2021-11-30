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

#ifndef LITEHTML_CSS_RANGE_H__
#define LITEHTML_CSS_RANGE_H__

#include <vector>

#include "litehtml/css/css_component_value.h"
#include "litehtml/css/css_token.h"

namespace litehtml {

template <typename T>
class CSSRange {
protected:
    T** begin_;

    T** end_;

public:
    CSSRange() = delete;

    explicit CSSRange(std::vector<T*>& range)
    : begin_(range.data())
    , end_(range.data() + range.size())
    {
    }


    CSSRange(T** begin, T** end)
    : begin_(begin)
    , end_(end)
    {
    }

    T* consume()
    {
        if (begin_ == end_) {
            return eof_value();
        }
        return *begin_++;
    }

    void reconsume()
    {
        begin_--;
    }

    T* peek()
    {
        if (begin_ == end_) {
            return eof_value();
        }
        return *begin_;
    }

    T** position()
    {
        return begin_;
    }

    static T* eof_value();
};

using CSSComponentValueRange = CSSRange<CSSComponentValue>;

using CSSTokenRange = CSSRange<CSSToken>;

} // namespace litehtml

#endif // LITEHTML_CSS_COMPONENT_VALUE_RANGE_H__
