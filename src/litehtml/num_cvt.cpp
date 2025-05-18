// Copyright (c) 2013, Yuri Kobets (tordex)
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

#include "litehtml/num_cvt.h"

#include <vector>

#include "litehtml/types.h"
#include "litehtml/utf8_strings.h"

namespace litehtml {

static std::vector<char> latin_lower = {'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z'};
static std::vector<char> latin_upper = {'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z'};
static std::vector<std::wstring> greek_lower = {L"α",
    L"β",
    L"γ",
    L"δ",
    L"ε",
    L"ζ",
    L"η",
    L"θ",
    L"ι",
    L"κ",
    L"λ",
    L"μ",
    L"ν",
    L"ξ",
    L"ο",
    L"π",
    L"ρ",
    L"σ",
    L"τ",
    L"υ",
    L"φ",
    L"χ",
    L"ψ",
    L"ω"};

static std::string to_mapped_alpha(int num, const std::vector<char>& map)
{
    int dividend = num;
    std::string out;
    int modulo;

    while (dividend > 0) {
        modulo = (dividend - 1) % map.size();
        out = map[modulo] + out;
        dividend = (int)((dividend - modulo) / map.size());
    }

    return out;
}

static std::string to_mapped_alpha(int num, const std::vector<std::wstring>& map)
{
    int dividend = num;
    std::string out;
    int modulo;

    while (dividend > 0) {
        modulo = (dividend - 1) % map.size();
        out = litehtml_from_wchar(map[modulo]).c_str() + out;
        dividend = (int)((dividend - modulo) / map.size());
    }

    return out;
}

std::string num_cvt::to_latin_lower(int val)
{
    return to_mapped_alpha(val, latin_lower);
}

std::string num_cvt::to_latin_upper(int val)
{
    return to_mapped_alpha(val, latin_upper);
}

std::string num_cvt::to_greek_lower(int val)
{
    return to_mapped_alpha(val, greek_lower);
}

std::string num_cvt::to_roman_lower(int value)
{
    struct romandata_t {
        int value;
        const char* numeral;
    };
    const struct romandata_t romandata[] = {
        {1000, "m"},
        {900, "cm"},
        {500, "d"},
        {400, "cd"},
        {100, "c"},
        {90, "xc"},
        {50, "l"},
        {40, "xl"},
        {10, "x"},
        {9, "ix"},
        {5, "v"},
        {4, "iv"},
        {1, "i"},
        {0, nullptr} // end marker
    };

    std::string result;
    for (const romandata_t* current = romandata; current->value > 0; ++current) {
        while (value >= current->value) {
            result += current->numeral;
            value -= current->value;
        }
    }
    return result;
}

std::string num_cvt::to_roman_upper(int value)
{
    struct romandata_t {
        int value;
        const char* numeral;
    };
    const struct romandata_t romandata[] = {
        {1000, "M"},
        {900, "CM"},
        {500, "D"},
        {400, "CD"},
        {100, "C"},
        {90, "XC"},
        {50, "L"},
        {40, "XL"},
        {10, "X"},
        {9, "IX"},
        {5, "V"},
        {4, "IV"},
        {1, "I"},
        {0, nullptr} // end marker
    };

    std::string result;
    for (const romandata_t* current = romandata; current->value > 0; ++current) {
        while (value >= current->value) {
            result += current->numeral;
            value -= current->value;
        }
    }
    return result;
}

} // namespace litehtml
