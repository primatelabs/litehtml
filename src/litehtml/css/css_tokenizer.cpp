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

#include "litehtml/css/css_tokenizer.h"

#include <assert.h>

#include <iostream>

#include "litehtml/css/css_number.h"
#include "litehtml/text.h"

namespace litehtml {

namespace {

bool is_eof(char32_t c)
{
    return (c == '\0');
}

// https://www.w3.org/TR/css-syntax-3/#name-start-code-point
bool is_name_start_code_point(char32_t c)
{
    static const uint32_t start_lookup[] = {0x00000000,
        0x00000000,
        0x87fffffe,
        0x07fffffe};

    if (is_non_ascii_code_point(c)) {
        return true;
    }
    return lookup(start_lookup, c);
}

// https://www.w3.org/TR/css-syntax-3/#name-code-point
bool is_name_code_point(char32_t c)
{
    static const uint32_t name_lookup[] = {0x00000000,
        0x03ff2000,
        0x87fffffe,
        0x07fffffe};

    if (is_non_ascii_code_point(c)) {
        return true;
    }
    return lookup(name_lookup, c);
}

// Returns true if the two code points are a valid escape.
// https://www.w3.org/TR/css-syntax-3/#starts-with-a-valid-escape
bool is_valid_escape(char32_t c0, char32_t c1)
{
    if (c0 != '\\') {
        return false;
    }
    if (is_newline(c1)) {
        return false;
    }
    return true;
}

// Returns true if the three code points would start an identifier.
// https://www.w3.org/TR/css-syntax-3/#would-start-an-identifier
bool would_start_identifier(char32_t c0,
    char32_t c1,
    char32_t c2)
{
    if (c0 == '-') {
        if (is_name_start_code_point(c1) || c1 == '-' ||
            is_valid_escape(c1, c2)) {
            return true;
        } else {
            return false;
        }
    } else if (is_name_start_code_point(c1)) {
        return true;
    } else if (c0 == '\\') {
        return is_valid_escape(c1, c2);
    }

    return false;
}

// Returns true if the three code points would start a number.
// https://www.w3.org/TR/css-syntax-3/#starts-with-a-number
bool would_start_number(char32_t c0,
    char32_t c1,
    char32_t c2)
{
    if (c0 == '+' || c0 == '-') {
        if (is_digit(c1)) {
            return true;
        } else if (c1 == '.' && is_digit(c2)) {
            return true;
        }
    } else if (c0 == '.' && is_digit(c1)) {
        return true;
    } else if (is_digit(c0)) {
        return true;
    }

    return false;
}

} // namespace

CSSTokenizer::CSSTokenizer(String input)
: stream_(input)
{
    while (true) {
        CSSToken* token = next();
        tokens_.push_back(token);
        if (token->type() == kCSSTokenEOF) {
            break;
        }
    }
}

CSSTokenizer::~CSSTokenizer()
{
    for (auto token : tokens_) {
        delete token;
    }
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
CSSToken* CSSTokenizer::consume_whitespace()
{
    while (true) {
        char c = stream_.consume();
        if (!is_whitespace(c)) {
            stream_.replace(c);
            break;
        }
    }

    return new CSSToken(kCSSTokenWhitespace);
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
CSSToken* CSSTokenizer::consume_number_sign(char c0)
{
    char c1 = stream_.peek(0);
    char c2 = stream_.peek(1);
    char c3 = stream_.peek(1);
    CSSToken* result = nullptr;

    if (is_name_code_point(c1) || is_valid_escape(c1, c2)) {
        if (would_start_identifier(c1, c2, c3)) {
            // TODO: Set the hash token type flag to `id`.
        }
        stream_.advance();
        String value = consume_name(c1);
        result = new CSSToken(kCSSTokenHash, value);
    }

    if (!result) {
        result = new CSSToken(kCSSTokenDelim, c0);
    }

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
CSSToken* CSSTokenizer::consume_plus_sign(char c0)
{
    char c1 = stream_.peek(0);
    char c2 = stream_.peek(1);
    CSSToken* result = nullptr;

    if (would_start_number(c0, c1, c2)) {
        result = consume_numeric(c0);
    } else {
        result = new CSSToken(kCSSTokenDelim, c0);
    }

    assert(result && result->type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
CSSToken* CSSTokenizer::consume_hyphen_minus(char c0)
{
    char c1 = stream_.peek(0);
    char c2 = stream_.peek(1);
    CSSToken* result = nullptr;

    if (would_start_number(c0, c1, c2)) {
        result = consume_numeric(c0);
    } else if (c1 == '-' && c2 == '>') {
        result = new CSSToken(kCSSTokenCDC);
    } else if (would_start_identifier(c0, c1, c2)) {
        result = consume_ident(c0);
    } else {
        result = new CSSToken(kCSSTokenDelim, c0);
    }

    assert(result && result->type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
CSSToken* CSSTokenizer::consume_full_stop(char c0)
{
    char c1 = stream_.peek(0);
    char c2 = stream_.peek(1);
    CSSToken* result = nullptr;

    if (would_start_number(c0, c1, c2)) {
        result = consume_numeric(c0);
    } else {
        result = new CSSToken(kCSSTokenDelim, c0);
    }

    assert(result && result->type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
CSSToken* CSSTokenizer::consume_less_than(char c0)
{
    char c1 = stream_.peek(0);
    char c2 = stream_.peek(1);
    char c3 = stream_.peek(2);
    CSSToken* result = nullptr;

    if (c1 == '!' && c2 == '-' && c3 == '-') {
        stream_.advance(3);
        result = new CSSToken(kCSSTokenCDO);
    } else {
        result = new CSSToken(kCSSTokenDelim, c0);
    }

    assert(result && result->type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
CSSToken* CSSTokenizer::consume_at(char c0)
{
    char c1 = stream_.peek(0);
    char c2 = stream_.peek(1);
    char c3 = stream_.peek(2);
    CSSToken* result = nullptr;

    if (would_start_identifier(c1, c2, c3)) {
        stream_.advance();
        String name = consume_name(c1);
        result = new CSSToken(kCSSTokenAtKeyword, name);
    } else {
        result = new CSSToken(kCSSTokenDelim, c0);
    }

    assert(result && result->type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
CSSToken* CSSTokenizer::consume_backslash(char c0)
{
    char c1 = stream_.peek(0);
    CSSToken* result = nullptr;

    if (is_valid_escape(c0, c1)) {
        result = consume_ident(c0);
    } else {
        // FIXME: Parse error
        result = new CSSToken(kCSSTokenDelim, c0);
    }

    assert(result && result->type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-comment
void CSSTokenizer::consume_comment()
{
    while (true) {
        if (stream_.peek(0) == '*' && stream_.peek(1) == '/') {
            stream_.advance(2);
            break;
        } else if (stream_.peek(0) == 0) {
            // FIXME: Indicate a parse error occurred.
            break;
        } else {
            stream_.advance(1);
        }
    }
}

// https://www.w3.org/TR/css-syntax-3/#consume-numeric-token
CSSToken* CSSTokenizer::consume_numeric(char first)
{
    return new CSSToken(kCSSTokenNumber, consume_number(first));
}


// https://www.w3.org/TR/css-syntax-3/#consume-ident-like-token
CSSToken* CSSTokenizer::consume_ident(char first)
{
    String name = consume_name(first);

    if (name == "url" && stream_.peek(0) == '(') {
        stream_.consume();
        while (is_whitespace(stream_.peek(0)) && is_whitespace(stream_.peek(1))) {
            stream_.consume();
        }

        // This part of the CSS tokenization specification is confusing:
        //
        //   If the next one or two input code points are U+0022 QUOTATION
        //   MARK ("), U+0027 APOSTROPHE ('), or whitespace followed by U+0022
        //   QUOTATION MARK (") or U+0027 APOSTROPHE ('), then create a
        //   <function-token> with its value set to string and return it.
        //   Otherwise, consume a url token, and return it.
        //
        // The part about "the next one or two input code points" is unclear.
        // The code below should implement this but without good test cases
        // that demonstrate the edge cases I can't be certain.

        if (stream_.peek(0) == '\"' || stream_.peek(0) == '\'') {
            return new CSSToken(kCSSTokenFunction, name);
        }

        if (is_whitespace(stream_.peek(0)) && (stream_.peek(1) == '\"' || stream_.peek(1) == '\'')) {
            return new CSSToken(kCSSTokenFunction, name);
        }

        return consume_url();
    }

    if (stream_.peek(0) == '(') {
        stream_.consume();
        return new CSSToken(kCSSTokenFunction, name);
    }

    return new CSSToken(kCSSTokenIdent, name);
}

// https://www.w3.org/TR/css-syntax-3/#consume-string-token
CSSToken* CSSTokenizer::consume_string(char ending)
{
    CSSToken* token = nullptr;
    String value;

    while (true) {
        char c = stream_.consume();
        if (c == ending) {
            token = new CSSToken(kCSSTokenString, value);
            break;
        } else if (c == '\0') {
            // FIXME: Indicate a parse error occurred.
            token = new CSSToken(kCSSTokenString, value);
            break;
        } else if (is_newline(c)) {
            token = new CSSToken(kCSSTokenBadString);
            break;
        } else if (c == '\\') {
            if (stream_.peek(0) == '\0') {
                // Do nothing
            } else if (stream_.peek(0) == '\n') {
                value.push_back(c);
            } else {
                c = consume_escape();
                value.push_back(c);
            }
        } else {
            value.push_back(c);
        }
    }

    assert(token);
    assert(token->type() == kCSSTokenString || token->type() == kCSSTokenBadString);

    return token;
}

// https://www.w3.org/TR/css-syntax-3/#consume-url-token
CSSToken* CSSTokenizer::consume_url()
{
    // 1. Create a <url-token> with its value set to the empty string.
    String value;// token(kCSSTokenURL);

    // 2. Consume as much whitespace as possible.
    while (is_whitespace(stream_.peek(0))) {
        stream_.consume();
    }

    while (true) {
        char c = stream_.consume();
        if (c == ')') {
            break;
        } else if (c == '\0') {
            // FIXME: Indicate a parse error occurred.
            break;
        } else if (is_whitespace(c)) {
            while (is_whitespace(stream_.peek(0))) {
                stream_.consume();
            }
            c = stream_.consume();
            if (c == ')') {
                break;
            } else if (c == '\0') {
                // FIXME: Indicate a parse error occurred.
                break;
            } else {
                // FIXME: Indicate a parse error occurred.
                return consume_bad_url();
            }
        } else if (c == '"' || c == '\'' || c == '(' || is_non_printable_code_point(c)) {
            // FIXME: Indicate a parse error occurred.
            return consume_bad_url();
        } else {
            value.push_back(c);
        }

    }

    return new CSSToken(kCSSTokenURL, value);
}

// https://www.w3.org/TR/css-syntax-3/#consume-escaped-code-point
char32_t CSSTokenizer::consume_escape()
{
    char32_t result = 0;
    char32_t c = stream_.consume();

    if (is_hex_digit(c)) {
        // FIXME: Implement
    } else if (is_eof(c)) {
        // FIXME: Parse error
        // FIXME: Return replacement character
    } else {
        result = c;
    }

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-name
String CSSTokenizer::consume_name(char first)
{
    String result;
    result.push_back(first);

    while (true) {
        char c = stream_.consume();
        if (is_name_code_point(c)) {
            result.push_back(c);
        } else if (c == '\\') {
            c = consume_escape();
        } else {
            stream_.replace(c);
            break;
        }
    }

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-number
CSSNumber CSSTokenizer::consume_number(char first)
{
    CSSNumberValueType type = kCSSIntegerValue;
    String repr;

    if (first == '+' || first == '-') {
        repr.push_back(first);
    } else {
        stream_.replace(first);
    }

    while (is_digit(stream_.peek(0))) {
        repr.push_back(stream_.consume());
    }

    if (stream_.peek(0) == '.' && is_digit(stream_.peek(1))) {
        type = kCSSNumberValue;
        repr.push_back(stream_.consume());
        repr.push_back(stream_.consume());
        while (is_digit(stream_.peek(0))) {
            repr.push_back(stream_.consume());
        }
    }

#if 0
    if (stream_.peek(0) == 'E' || stream_.peek(0) == 'e') {
        if (stream_.peek(1) == '+' || stream_.peek(1) == '-') {
            if (is_digit(stream_.peek(2))) {
                type = kCSSNumberValue;
                repr.push_back(stream_.consume());
                repr.push_back(stream_.consume());
                repr.push_back(stream_.consume());
                while (is_digit(stream_.peek(0))) {
                    repr.push_back(stream_.consume());
                }
            }
        } else if (is_digit(stream_.peek(1))) {
            type = kCSSNumberValue;
            repr.push_back(stream_.consume());
            repr.push_back(stream_.consume());
            while (is_digit(stream_.peek(0))) {
                repr.push_back(stream_.consume());
            }
        }
    }
#endif

    return CSSNumber(type, std::stod(repr));
}

// https://www.w3.org/TR/css-syntax-3/#consume-remnants-of-bad-url
CSSToken* CSSTokenizer::consume_bad_url()
{
    while (true) {
        char c = stream_.consume();

        if (c == '(' || c == '\0') {
            break;
        } else if (is_valid_escape(c, stream_.peek(0))) {
            consume_escape();
        }
    }

    return new CSSToken(kCSSTokenBadURL);
}

CSSToken* CSSTokenizer::next()
{
    while (true) {
        if (stream_.peek(0) == '/' && stream_.peek(1) == '*') {
            stream_.advance(2);
            consume_comment();
        } else {
            break;
        }
    }

    char c = stream_.consume();
    switch (c) {
        case '\0':
            return new CSSToken(kCSSTokenEOF);

        case '\t':
        case '\n':
        case '\r':
        case '\f':
        case ' ':
            return consume_whitespace();

        case '\"':
        case '\'':
            return consume_string(c);

        case '#':
            return consume_number_sign(c);

        case '+':
            return consume_plus_sign(c);

        case ',':
            return new CSSToken(kCSSTokenComma);

        case '-':
            return consume_hyphen_minus(c);

        case '.':
            return consume_full_stop(c);

        case ':':
            return new CSSToken(kCSSTokenColon);

        case ';':
            return new CSSToken(kCSSTokenSemicolon);

        case '<':
            return consume_less_than(c);

        case '@':
            return consume_at(c);

        case '\\':
            return consume_backslash(c);

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return consume_numeric(c);

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case '_':
            return consume_ident(c);

        case '(':
            return new CSSToken(kCSSTokenOpenRoundBracket);
        case ')':
            return new CSSToken(kCSSTokenCloseRoundBracket);

        case '[':
            return new CSSToken(kCSSTokenOpenSquareBracket);
        case ']':
            return new CSSToken(kCSSTokenCloseSquareBracket);

        case '{':
            return new CSSToken(kCSSTokenOpenBrace);
        case '}':
            return new CSSToken(kCSSTokenCloseBrace);

        default:
            return new CSSToken(kCSSTokenDelim, c);
    }
}

} // namespace litehtml
