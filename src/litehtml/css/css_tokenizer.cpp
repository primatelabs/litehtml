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
    if (c0 != _t('\\')) {
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
    if (c0 == _t('-')) {
        if (is_name_start_code_point(c1) || c1 == _t('-') ||
            is_valid_escape(c1, c2)) {
            return true;
        } else {
            return false;
        }
    } else if (is_name_start_code_point(c1)) {
        return true;
    } else if (c0 == _t('\\')) {
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
    if (c0 == _t('+') || c0 == _t('-')) {
        if (is_digit(c1)) {
            return true;
        } else if (c1 == _t('.') && is_digit(c2)) {
            return true;
        }
    } else if (c0 == _t('.') && is_digit(c1)) {
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
        tchar_t c = stream_.consume();
        if (!is_whitespace(c)) {
            stream_.replace(c);
            break;
        }
    }

    return new CSSToken(kCSSTokenWhitespace);
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
CSSToken* CSSTokenizer::consume_number_sign(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
    tchar_t c2 = stream_.peek(1);
    tchar_t c3 = stream_.peek(1);
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
CSSToken* CSSTokenizer::consume_plus_sign(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
    tchar_t c2 = stream_.peek(1);
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
CSSToken* CSSTokenizer::consume_hyphen_minus(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
    tchar_t c2 = stream_.peek(1);
    CSSToken* result = nullptr;

    if (would_start_number(c0, c1, c2)) {
        result = consume_numeric(c0);
    } else if (c1 == _t('-') && c2 == _t('>')) {
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
CSSToken* CSSTokenizer::consume_full_stop(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
    tchar_t c2 = stream_.peek(1);
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
CSSToken* CSSTokenizer::consume_less_than(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
    tchar_t c2 = stream_.peek(1);
    tchar_t c3 = stream_.peek(2);
    CSSToken* result = nullptr;

    if (c1 == _t('!') && c2 == _t('-') && c3 == _t('-')) {
        stream_.advance(3);
        result = new CSSToken(kCSSTokenCDO);
    } else {
        result = new CSSToken(kCSSTokenDelim, c0);
    }

    assert(result && result->type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
CSSToken* CSSTokenizer::consume_at(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
    tchar_t c2 = stream_.peek(1);
    tchar_t c3 = stream_.peek(2);
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
CSSToken* CSSTokenizer::consume_backslash(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
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
        if (stream_.peek(0) == _t('*') && stream_.peek(1) == _t('/')) {
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
CSSToken* CSSTokenizer::consume_numeric(tchar_t first)
{
    return new CSSToken(kCSSTokenNumber, consume_number(first));
}


// https://www.w3.org/TR/css-syntax-3/#consume-ident-like-token
CSSToken* CSSTokenizer::consume_ident(tchar_t first)
{
    String name = consume_name(first);

    if (name == _t("url") && stream_.peek(0) == _t('(')) {
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

        if (stream_.peek(0) == _t('\"') || stream_.peek(0) == _t('\'')) {
            return new CSSToken(kCSSTokenFunction, name);
        }

        if (is_whitespace(stream_.peek(0)) && (stream_.peek(1) == _t('\"') || stream_.peek(1) == _t('\''))) {
            return new CSSToken(kCSSTokenFunction, name);
        }

        return consume_url();
    }

    if (stream_.peek(0) == _t('(')) {
        stream_.consume();
        return new CSSToken(kCSSTokenFunction, name);
    }

    return new CSSToken(kCSSTokenIdent, name);
}

// https://www.w3.org/TR/css-syntax-3/#consume-string-token
CSSToken* CSSTokenizer::consume_string(tchar_t ending)
{
    CSSToken* token = nullptr;
    String value;

    while (true) {
        tchar_t c = stream_.consume();
        if (c == ending) {
            token = new CSSToken(kCSSTokenString, value);
            break;
        } else if (c == _t('\0')) {
            // FIXME: Indicate a parse error occurred.
            token = new CSSToken(kCSSTokenString, value);
            break;
        } else if (is_newline(c)) {
            token = new CSSToken(kCSSTokenBadString);
            break;
        } else if (c == _t('\\')) {
            if (stream_.peek(0) == _t('\0')) {
                // Do nothing
            } else if (stream_.peek(0) == _t('\n')) {
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
        tchar_t c = stream_.consume();
        if (c == _t(')')) {
            break;
        } else if (c == _t('\0')) {
            // FIXME: Indicate a parse error occurred.
            break;
        } else if (is_whitespace(c)) {
            while (is_whitespace(stream_.peek(0))) {
                stream_.consume();
            }
            c = stream_.consume();
            if (c == _t(')')) {
                break;
            } else if (c == _t('\0')) {
                // FIXME: Indicate a parse error occurred.
                break;
            } else {
                // FIXME: Indicate a parse error occurred.
                return consume_bad_url();
            }
        } else if (c == _t('"') || c == _t('\'') || c == _t('(') || is_non_printable_code_point(c)) {
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
String CSSTokenizer::consume_name(tchar_t first)
{
    String result;
    result.push_back(first);

    while (true) {
        tchar_t c = stream_.consume();
        if (is_name_code_point(c)) {
            result.push_back(c);
        } else if (c == _t('\\')) {
            c = consume_escape();
        } else {
            stream_.replace(c);
            break;
        }
    }

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-number
CSSNumber CSSTokenizer::consume_number(tchar_t first)
{
    CSSNumberValueType type = kCSSIntegerValue;
    String repr;

    if (first == _t('+') || first == _t('-')) {
        repr.push_back(first);
    } else {
        stream_.replace(first);
    }

    while (is_digit(stream_.peek(0))) {
        repr.push_back(stream_.consume());
    }

    if (stream_.peek(0) == _t('.') && is_digit(stream_.peek(1))) {
        type = kCSSNumberValue;
        repr.push_back(stream_.consume());
        repr.push_back(stream_.consume());
        while (is_digit(stream_.peek(0))) {
            repr.push_back(stream_.consume());
        }
    }

#if 0
    if (stream_.peek(0) == _t('E') || stream_.peek(0) == _t('e')) {
        if (stream_.peek(1) == _t('+') || stream_.peek(1) == _t('-')) {
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
        tchar_t c = stream_.consume();

        if (c == _t('(') || c == _t('\0')) {
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
        if (stream_.peek(0) == _t('/') && stream_.peek(1) == _t('*')) {
            stream_.advance(2);
            consume_comment();
        } else {
            break;
        }
    }

    tchar_t c = stream_.consume();
    switch (c) {
        case _t('\0'):
            return new CSSToken(kCSSTokenEOF);

        case _t('\t'):
        case _t('\n'):
        case _t('\r'):
        case _t('\f'):
        case _t(' '):
            return consume_whitespace();

        case _t('\"'):
        case _t('\''):
            return consume_string(c);

        case _t('#'):
            return consume_number_sign(c);

        case _t('+'):
            return consume_plus_sign(c);

        case _t(','):
            return new CSSToken(kCSSTokenComma);

        case _t('-'):
            return consume_hyphen_minus(c);

        case _t('.'):
            return consume_full_stop(c);

        case _t(':'):
            return new CSSToken(kCSSTokenColon);

        case _t(';'):
            return new CSSToken(kCSSTokenSemicolon);

        case _t('<'):
            return consume_less_than(c);

        case _t('@'):
            return consume_at(c);

        case _t('\\'):
            return consume_backslash(c);

        case _t('0'):
        case _t('1'):
        case _t('2'):
        case _t('3'):
        case _t('4'):
        case _t('5'):
        case _t('6'):
        case _t('7'):
        case _t('8'):
        case _t('9'):
            return consume_numeric(c);

        case _t('A'):
        case _t('B'):
        case _t('C'):
        case _t('D'):
        case _t('E'):
        case _t('F'):
        case _t('G'):
        case _t('H'):
        case _t('I'):
        case _t('J'):
        case _t('K'):
        case _t('L'):
        case _t('M'):
        case _t('N'):
        case _t('O'):
        case _t('P'):
        case _t('Q'):
        case _t('R'):
        case _t('S'):
        case _t('T'):
        case _t('U'):
        case _t('V'):
        case _t('W'):
        case _t('X'):
        case _t('Y'):
        case _t('Z'):
        case _t('a'):
        case _t('b'):
        case _t('c'):
        case _t('d'):
        case _t('e'):
        case _t('f'):
        case _t('g'):
        case _t('h'):
        case _t('i'):
        case _t('j'):
        case _t('k'):
        case _t('l'):
        case _t('m'):
        case _t('n'):
        case _t('o'):
        case _t('p'):
        case _t('q'):
        case _t('r'):
        case _t('s'):
        case _t('t'):
        case _t('u'):
        case _t('v'):
        case _t('w'):
        case _t('x'):
        case _t('y'):
        case _t('_'):
            return consume_ident(c);

        case _t('('):
            return new CSSToken(kCSSTokenOpenRoundBracket);
        case _t(')'):
            return new CSSToken(kCSSTokenCloseRoundBracket);

        case _t('['):
            return new CSSToken(kCSSTokenOpenSquareBracket);
        case _t(']'):
            return new CSSToken(kCSSTokenCloseSquareBracket);

        case _t('{'):
            return new CSSToken(kCSSTokenOpenBrace);
        case _t('}'):
            return new CSSToken(kCSSTokenCloseBrace);

        default:
            return new CSSToken(kCSSTokenDelim, c);
    }
}

} // namespace litehtml
