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

#include "litehtml/css/css_length.h"

#include "litehtml/html.h"

namespace litehtml {

void css_length::fromString(const tstring& str, const tstring& predefs, int defValue)
{
    // TODO: Make support for calc
    if (str.substr(0, 4) == _t("calc")) {
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

        tstring num;
        tstring un;
        bool is_unit = false;
        for (tstring::const_iterator chr = str.begin(); chr != str.end(); chr++) {
            if (!is_unit) {
                if (t_isdigit(*chr) || *chr == _t('.') || *chr == _t('+') ||
                    *chr == _t('-')) {
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
            m_value = (float)t_strtod(num.c_str(), nullptr);
            m_units = (css_units)value_index(un.c_str(),
                css_units_strings,
                css_units_none);
        } else {
            // not a number so it is predefined
            m_is_predefined = true;
            m_predef = defValue;
        }
    }
}

} // namespace litehtml
