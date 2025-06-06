// Copyright (c) 2013, Yuri Kobets (tordex)
// Copyright (c) 2020-2021 Primate Labs Inc.
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

#include "litehtml/css/css_length.h"

#include "litehtml/html.h"

namespace litehtml {

void CSSLength::parse_length_string(const std::string& str, const std::string& predefs, int defValue)
{
    // TODO: Make support for calc
    if (str.substr(0, 4) == "calc") {
        m_is_predefined = true;
        m_predef = 0;
        return;
    }

    int predef = value_index(str.c_str(), predefs.c_str(), -1);
    if (predef >= 0) {
        m_is_predefined = true;
        m_predef = predef;
    } else {
        m_is_predefined = false;

        std::string num;
        std::string un;
        bool is_unit = false;
        for (std::string::const_iterator chr = str.begin(); chr != str.end(); chr++) {
            if (!is_unit) {
                if (isdigit(*chr) || *chr == '.' || *chr == '+' ||
                    *chr == '-') {
                    num += *chr;
                } else {
                    is_unit = true;
                }
            }
            if (is_unit) {
                un += *chr;
            }
        }
        if (!num.empty()) {
            m_value = (float)strtod(num.c_str(), nullptr);
            m_units = (CSSUnits)value_index(un.c_str(),
                CSS_UNITS_STRINGS,
                kCSSUnitsNone);
        } else {
            // not a number so it is predefined
            m_is_predefined = true;
            m_predef = defValue;
        }
    }
}


void CSSLength::parse_length_string(const std::string& str, const KeywordVector& keywords, int default_keyword)
{
    for (auto& keyword : keywords) {
        if (!t_strcasecmp(keyword.first.c_str(), str.c_str())) {
            m_is_predefined = true;
            m_predef = default_keyword;
            return;
        }
    }

    m_is_predefined = false;

    std::string num;
    std::string un;
    bool is_unit = false;
    for (std::string::const_iterator chr = str.begin(); chr != str.end(); chr++) {
        if (!is_unit) {
            if (isdigit(*chr) || *chr == '.' || *chr == '+' ||
                *chr == '-') {
                num += *chr;
            } else {
                is_unit = true;
            }
        }
        if (is_unit) {
            un += *chr;
        }
    }
    if (!num.empty()) {
        m_value = (float)strtod(num.c_str(), nullptr);
        m_units = (CSSUnits)value_index(un.c_str(),
            CSS_UNITS_STRINGS,
            kCSSUnitsNone);
    } else {
        // not a number so it is predefined
        m_is_predefined = true;
        m_predef = default_keyword;
    }
}

std::basic_ostream<String::value_type>& operator<<(
    std::basic_ostream<String::value_type>& os,
    const CSSLength& length)
{
    if (length.is_predefined()) {
        os << length.predef();
    } else {
        os << length.val() << " " << length.units();
    }
    return os;
}

} // namespace litehtml
