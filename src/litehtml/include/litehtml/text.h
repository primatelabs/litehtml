// Copyright (C) 2020-2022 Primate Labs Inc.
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

#pragma once

#include <stdint.h>

namespace litehtml {

inline bool lookup(const uint32_t* table, char32_t c)
{
    return table[c >> 5] & (1 << (c & 0x1f));
}

inline bool is_non_ascii_code_point(char32_t c)
{
    return (c > 127);
}

inline bool is_digit(char32_t c)
{
    static const uint32_t digit_lookup[] = {
        0x00000000,
        0x03ff0000,
        0x00000000,
        0x00000000};

    if (is_non_ascii_code_point(c)) {
        return false;
    }
    return lookup(digit_lookup, c);
}

inline bool is_hex_digit(char32_t c)
{
    static const uint32_t hex_digit_lookup[] = {
        0x00000000,
        0x03ff0000,
        0x0000007e,
        0x0000007e};

    if (is_non_ascii_code_point(c)) {
        return false;
    }
    return lookup(hex_digit_lookup, c);
}

inline bool is_newline(char32_t c)
{
    static const uint32_t newline_lookup[] = {
        0x00003400,
        0x00000000,
        0x00000000,
        0x00000000};

    if (is_non_ascii_code_point(c)) {
        return false;
    }
    return lookup(newline_lookup, c);
}

inline bool is_non_printable_code_point(char32_t c)
{
    static const uint32_t non_printable_lookup[] = {
        0xffffc9ff,
        0x00000000,
        0x00000000,
        0x80000000};

    if (is_non_ascii_code_point(c)) {
        return false;
    }
    return lookup(non_printable_lookup, c);
}

inline bool is_whitespace(char32_t c)
{
    static const uint32_t whitespace_lookup[] = {
        0x00003600,
        0x00000001,
        0x00000000,
        0x00000000};

    if (is_non_ascii_code_point(c)) {
        return false;
    }
    return lookup(whitespace_lookup, c);
}

} // namespace litehtml
