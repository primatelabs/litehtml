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

#include "litehtml/css/css_number.h"
#include "litehtml/css/css_token.h"
#include "litehtml/css/css_tokenizer_input_stream.h"

namespace litehtml {

class CSSTokenizer {
protected:
    CSSTokenizerInputStream stream_;

    std::vector<CSSToken*> tokens_;

    int offset_;

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    CSSToken* consume_whitespace();

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    CSSToken* consume_number_sign(char c0);

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    CSSToken* consume_plus_sign(char c0);

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    CSSToken* consume_hyphen_minus(char c0);

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    CSSToken* consume_full_stop(char c0);

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    CSSToken* consume_less_than(char c0);

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    CSSToken* consume_at(char c0);

    // https://www.w3.org/TR/css-syntax-3/#consume-token
    CSSToken* consume_backslash(char first);

    // https://www.w3.org/TR/css-syntax-3/#consume-comment
    void consume_comment();

    // https://www.w3.org/TR/css-syntax-3/#consume-number
    CSSToken* consume_numeric(char first);

    // https://www.w3.org/TR/css-syntax-3/#consume-ident-like-token
    CSSToken* consume_ident(char first);

    // https://www.w3.org/TR/css-syntax-3/#consume-string-token
    CSSToken* consume_string(char opening);

    // https://www.w3.org/TR/css-syntax-3/#consume-url-token
    CSSToken* consume_url();

    // https://www.w3.org/TR/css-syntax-3/#consume-escaped-code-point
    char32_t consume_escape();

    // https://www.w3.org/TR/css-syntax-3/#consume-name
    String consume_name(char first);

    // https://www.w3.org/TR/css-syntax-3/#consume-number
    CSSNumber consume_number(char first);

    // https://www.w3.org/TR/css-syntax-3/#consume-remnants-of-bad-url
    CSSToken* consume_bad_url();

    CSSToken* next();

public:
    explicit CSSTokenizer(String input);

    ~CSSTokenizer();

    std::vector<CSSToken*>& tokens()
    {
        return tokens_;
    }

    CSSToken* consume();

    void reconsume();
};

} // namespace litehtml

#endif // LITEHTML_CSS_TOKENIZER_H__
