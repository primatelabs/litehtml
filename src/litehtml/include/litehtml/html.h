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

#ifndef LITEHTML_HTML_H__
#define LITEHTML_HTML_H__

#include <ctype.h>
#include <stdlib.h>

#include <algorithm>
#include <cstring>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "litehtml/background.h"
#include "litehtml/borders.h"
#include "litehtml/color.h"
#include "litehtml/element/html_element.h"
#include "litehtml/media_query.h"
#include "litehtml/string.h"
#include "litehtml/types.h"

namespace litehtml {

void trim(std::string& s);
void lcase(std::string& s);
int value_index(const std::string& val,
    const std::string& strings,
    int defValue = -1,
    char delim = ';');
bool value_in_list(const std::string& val,
    const std::string& strings,
    char delim = ';');
std::string::size_type find_close_bracket(const std::string& s,
    std::string::size_type off,
    char open_b = '(',
    char close_b = ')');
void split_string(const std::string& str,
    string_vector& tokens,
    const std::string& delims,
    const std::string& delims_preserve = "",
    const std::string& quote = "\"");
void join_string(std::string& str, const string_vector& tokens, const std::string& delims);

inline int round_f(float val)
{
    int int_val = (int)val;
    if (val - int_val >= 0.5f) {
        int_val++;
    }
    return int_val;
}

inline int round_d(double val)
{
    int int_val = (int)val;
    if (val - int_val >= 0.5) {
        int_val++;
    }
    return int_val;
}

} // namespace litehtml

#endif // LITEHTML_HTML_H__
