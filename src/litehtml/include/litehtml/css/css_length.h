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

#ifndef LITEHTML_CSS_LENGTH_H__
#define LITEHTML_CSS_LENGTH_H__

#include <ostream>

#include "litehtml/types.h"
#include "litehtml/css/css_property.h"

namespace litehtml {

class CSSLength {
    union {
        float m_value;
        int m_predef;
    };
    CSSUnits m_units;
    bool m_is_predefined;

public:
    CSSLength()
    : m_value(0)
    , m_units(kCSSUnitsNone)
    , m_is_predefined(false)
    {
    }

    explicit CSSLength(int predef)
    : m_predef(predef)
    , m_units(kCSSUnitsNone)
    , m_is_predefined(true)
    {
    }

    CSSLength(float value, CSSUnits units)
    : m_value(value)
    , m_units(units)
    , m_is_predefined(false)
    {
    }

    CSSLength(const CSSLength& other)
    {
        if (other.is_predefined()) {
            m_predef = other.m_predef;
        } else {
            m_value = other.m_value;
        }
        m_units = other.m_units;
        m_is_predefined = other.m_is_predefined;
    }

    CSSLength& operator=(const CSSLength& other)
    {
        if (other.is_predefined()) {
            m_predef = other.m_predef;
        } else {
            m_value = other.m_value;
        }
        m_units = other.m_units;
        m_is_predefined = other.m_is_predefined;
        return *this;
    }

    CSSLength& operator=(float val)
    {
        m_value = val;
        m_units = kCSSUnitsPx;
        m_is_predefined = false;
        return *this;
    }

    bool is_predefined() const
    {
        return m_is_predefined;
    }

    void predef(int val)
    {
        m_predef = val;
        m_is_predefined = true;
    }

    int predef() const
    {
        if (m_is_predefined) {
            return m_predef;
        }
        return 0;
    }

    void set_value(float val, CSSUnits units)
    {
        m_value = val;
        m_is_predefined = false;
        m_units = units;
    }

    float val() const
    {
        if (!m_is_predefined) {
            return m_value;
        }
        return 0;
    }

    CSSUnits units() const
    {
        return m_units;
    }

    int calc_percent(int width) const
    {
        if (!is_predefined()) {
            if (units() == kCSSUnitsPercent) {
                return (int)((double)width * (double)m_value / 100.0);
            } else {
                return (int)val();
            }
        }
        return 0;
    }

    void parse_length_string(const std::string& str,
        const std::string& predefs = "",
        int defValue = 0);

    void parse_length_string(const std::string& str,
        const KeywordVector& keywords,
        int default_keyword);
};

std::basic_ostream<String::value_type>& operator<<(
    std::basic_ostream<String::value_type>&,
    const CSSLength& length);

} // namespace litehtml

#endif // LITEHTML_CSS_LENGTH_H__
