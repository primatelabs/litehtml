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
//    * Neither the names of the copyright holders nor the names of their
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

#ifndef LITEHTML_TSTRING_VIEW_H__
#define LITEHTML_TSTRING_VIEW_H__

#include <ostream>

#include "litehtml/string.h"

namespace litehtml {

// StringView is a string reference type that provides a view into a string
// that is owned elsewhere (e.g., by a std::string object).

// StringView implements the same interface as std::base_string_view in the
// standard library.  When litehtml moves to C++17 consider replacing the
// StringView implementation with the standard library implementations
// (e.g., via a using statement).

class StringView {
public:
    using value_type = char;

    using pointer = char*;

    using const_pointer = const char*;

    using reference = char&;

    using const_reference = const char&;

    using iterator = const_pointer;

    using const_iterator = const_pointer;

    using size_type = size_t;

    using difference_type = ptrdiff_t;

public:
    StringView() = default;

    StringView(const StringView& other) = default;

    StringView(const_pointer s, size_type size)
    : data_(s)
    , size_(size)
    {
    }

    constexpr const_iterator begin() const
    {
        return data_;
    }

    constexpr const_iterator cbegin() const
    {
        return data_;
    }

    constexpr const_iterator end() const
    {
        return data_ + size_;
    }

    constexpr const_iterator cend() const
    {
        return data_ + size_;
    }

    constexpr const_reference operator[](size_type offset) const
    {
        return *(data_ + offset);
    }

    constexpr const_pointer data() const
    {
        return data_;
    }

    size_type size() const
    {
        return size_;
    }

    size_type length() const
    {
        return size_;
    }

    bool empty() const
    {
        return (size_ == 0);
    }

private:
    const_pointer data_ = nullptr;

    size_type size_ = 0;
};

std::basic_ostream<StringView::value_type>& operator<<(
    std::basic_ostream<StringView::value_type>&,
    StringView str);

} // namespace litehtml

#endif // LITEHTML_TSTRING_VIEW_H__
