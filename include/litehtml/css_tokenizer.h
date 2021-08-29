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

#ifndef LITEHTML_CSS_TOKENIZER_H__
#define LITEHTML_CSS_TOKENIZER_H__

#include "litehtml/css_number.h"
#include "litehtml/css_token.h"
#include "litehtml/css_tokenizer_input_stream.h"

namespace litehtml {

class css_tokenizer {
protected:
	css_tokenizer_input_stream stream_;

    std::vector<css_token> tokens_;

	int offset_;

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    css_token consume_whitespace();

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    css_token consume_number_sign(tchar_t c0);

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    css_token consume_plus_sign(tchar_t c0);

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    css_token consume_hyphen_minus(tchar_t c0);

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    css_token consume_full_stop(tchar_t c0);

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    css_token consume_less_than(tchar_t c0);

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    css_token consume_at(tchar_t c0);

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    css_token consume_backslash(tchar_t first);

    // https://www.w3.org/TR/css-syntax-3/#consume-comment
    void consume_comment();

    // https://www.w3.org/TR/css-syntax-3/#consume-number
    css_token consume_numeric(tchar_t first);

    // https://www.w3.org/TR/css-syntax-3/#consume-ident-like-token
    css_token consume_ident(tchar_t first);

    // https://www.w3.org/TR/css-syntax-3/#consume-string-token
    css_token consume_string(tchar_t opening);

    // https://www.w3.org/TR/css-syntax-3/#consume-url-token
    css_token consume_url();

    // https://www.w3.org/TR/css-syntax-3/#consume-escaped-code-point
    tchar_t consume_escape();

    // https://www.w3.org/TR/css-syntax-3/#consume-name
    tstring consume_name(tchar_t first);

    // https://www.w3.org/TR/css-syntax-3/#consume-number
    css_number consume_number(tchar_t first);

    css_token next();

public:
	explicit css_tokenizer(litehtml::tstring input);

    const std::vector<css_token>& tokens() const {
        return tokens_;
    }

    const css_token& consume();

    void reconsume();
};

} // namespace litehtml

#endif // LITEHTML_CSS_TOKENIZER_H__
