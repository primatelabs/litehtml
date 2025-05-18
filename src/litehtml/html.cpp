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

#include "litehtml/html.h"

#include "litehtml/element/html_element.h"
#include "litehtml/types.h"

namespace litehtml {

void trim(std::string& s)
{
    std::string::size_type pos = s.find_first_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        s.erase(s.begin(), s.begin() + pos);
    }
    pos = s.find_last_not_of(" \n\r\t");
    if (pos != std::string::npos) {
        s.erase(s.begin() + pos + 1, s.end());
    }
}

void lcase(std::string& s)
{
    for (std::string::iterator i = s.begin(); i != s.end(); i++) {
        (*i) = (char)tolower(*i);
    }
}

std::string::size_type find_close_bracket(const std::string& s,
    std::string::size_type off,
    char open_b,
    char close_b)
{
    int cnt = 0;
    for (std::string::size_type i = off; i < s.length(); i++) {
        if (s[i] == open_b) {
            cnt++;
        } else if (s[i] == close_b) {
            cnt--;
            if (!cnt) {
                return i;
            }
        }
    }
    return std::string::npos;
}

int value_index(const std::string& val, const std::string& strings, int defValue, char delim)
{
    if (val.empty() || strings.empty() || !delim) {
        return defValue;
    }

    int idx = 0;
    std::string::size_type delim_start = 0;
    std::string::size_type delim_end = strings.find(delim, delim_start);
    std::string::size_type item_len = 0;
    while (true) {
        if (delim_end == std::string::npos) {
            item_len = strings.length() - delim_start;
        } else {
            item_len = delim_end - delim_start;
        }
        if (item_len == val.length()) {
            if (val == strings.substr(delim_start, item_len)) {
                return idx;
            }
        }
        idx++;
        delim_start = delim_end;
        if (delim_start == std::string::npos)
            break;
        delim_start++;
        if (delim_start == strings.length())
            break;
        delim_end = strings.find(delim, delim_start);
    }
    return defValue;
}

bool value_in_list(const std::string& val, const std::string& strings, char delim)
{
    int idx = value_index(val, strings, -1, delim);
    if (idx >= 0) {
        return true;
    }
    return false;
}

void split_string(const std::string& str,
    string_vector& tokens,
    const std::string& delims,
    const std::string& delims_preserve,
    const std::string& quote)
{
    if (str.empty() || (delims.empty() && delims_preserve.empty())) {
        return;
    }

    std::string all_delims = delims + delims_preserve + quote;

    std::string::size_type token_start = 0;
    std::string::size_type token_end = str.find_first_of(all_delims, token_start);
    std::string::size_type token_len = 0;
    std::string token;
    while (true) {
        while (token_end != std::string::npos &&
               quote.find_first_of(str[token_end]) != std::string::npos) {
            if (str[token_end] == '(') {
                token_end = find_close_bracket(str, token_end, '(', ')');
            } else if (str[token_end] == '[') {
                token_end = find_close_bracket(str, token_end, '[', ']');
            } else if (str[token_end] == '{') {
                token_end = find_close_bracket(str, token_end, '{', '}');
            } else {
                token_end = str.find_first_of(str[token_end], token_end + 1);
            }
            if (token_end != std::string::npos) {
                token_end = str.find_first_of(all_delims, token_end + 1);
            }
        }

        if (token_end == std::string::npos) {
            token_len = std::string::npos;
        } else {
            token_len = token_end - token_start;
        }

        token = str.substr(token_start, token_len);
        if (!token.empty()) {
            tokens.push_back(token);
        }
        if (token_end != std::string::npos && !delims_preserve.empty() &&
            delims_preserve.find_first_of(str[token_end]) != std::string::npos) {
            tokens.push_back(str.substr(token_end, 1));
        }

        token_start = token_end;
        if (token_start == std::string::npos)
            break;
        token_start++;
        if (token_start == str.length())
            break;
        token_end = str.find_first_of(all_delims, token_start);
    }
}

void join_string(std::string& str, const string_vector& tokens, const std::string& delims)
{
    std::ostringstream oss;

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i != 0) {
            oss << delims;
        }
        oss << tokens[i];
    }

    str = oss.str();
}

} // namespace litehtml
