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

#include "litehtml/css_tokenizer.h"

#include <assert.h>

#include <iostream>

#include "litehtml/css_number.h"

namespace {

bool lookup(const uint32_t* table, litehtml::tchar_t c)
{
    return table[c >> 5] & (1 << (c & 0x1f));
}

// https://www.w3.org/TR/css-syntax-3/#non-ascii-code-point
bool is_non_ascii_code_point(litehtml::tchar_t c)
{
    return (c > 127);
}

bool is_eof(litehtml::tchar_t c)
{
    return (c == _t('\0'));
}

// https://www.w3.org/TR/css-syntax-3/#digit
bool is_digit(litehtml::tchar_t c)
{
    static const uint32_t digit_lookup[] = {0x00000000,
        0x03ff0000,
        0x00000000,
        0x00000000};

    if (is_non_ascii_code_point(c)) {
        return false;
    }
    return lookup(digit_lookup, c);
}

// https://www.w3.org/TR/css-syntax-3/#hex-digit
bool is_hex_digit(litehtml::tchar_t c)
{
    static const uint32_t hex_digit_lookup[] = {0x00000000,
        0x03ff0000,
        0x0000007e,
        0x0000007e};

    if (is_non_ascii_code_point(c)) {
        return false;
    }
    return lookup(hex_digit_lookup, c);
}

// https://www.w3.org/TR/css-syntax-3/#name-start-code-point
bool is_name_start_code_point(litehtml::tchar_t c)
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
bool is_name_code_point(litehtml::tchar_t c)
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

// https://www.w3.org/TR/css-syntax-3/#newline
bool is_newline(litehtml::tchar_t c)
{
    static const uint32_t newline_lookup[] = {0x00003400,
        0x00000000,
        0x00000000,
        0x00000000};

    if (is_non_ascii_code_point(c)) {
        return false;
    }
    return lookup(newline_lookup, c);
}

// https://www.w3.org/TR/css-syntax-3/#non-printable-code-point
bool is_non_printable_code_point(litehtml::tchar_t c)
{
    static const uint32_t non_printable_lookup[] = {0xffffc9ff,
        0x00000000,
        0x00000000,
        0x80000000};

    if (is_non_ascii_code_point(c)) {
        return false;
    }
    return lookup(non_printable_lookup, c);
}

// https://www.w3.org/TR/css-syntax-3/#whitespace
bool is_whitespace(litehtml::tchar_t c)
{
    static const uint32_t whitespace_lookup[] = {0x00003600,
        0x00000001,
        0x00000000,
        0x00000000};

    if (is_non_ascii_code_point(c)) {
        return false;
    }
    return lookup(whitespace_lookup, c);
}

// Returns true if the two code points are a valid escape.
// https://www.w3.org/TR/css-syntax-3/#starts-with-a-valid-escape
bool is_valid_escape(litehtml::tchar_t c0, litehtml::tchar_t c1)
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
bool would_start_identifier(litehtml::tchar_t c0,
    litehtml::tchar_t c1,
    litehtml::tchar_t c2)
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
bool would_start_number(litehtml::tchar_t c0,
    litehtml::tchar_t c1,
    litehtml::tchar_t c2)
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

namespace litehtml {

css_tokenizer::css_tokenizer(litehtml::tstring input)
: stream_(input)
{
    while (true) {
        css_token token = next();
        tokens_.push_back(token);
        if (token.type() == kCSSTokenEOF) {
            break;
        }
    }
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
css_token css_tokenizer::consume_whitespace()
{
    while (true) {
        tchar_t c = stream_.consume();
        if (!is_whitespace(c)) {
            stream_.replace(c);
            break;
        }
    }

    return css_token(kCSSTokenWhitespace);
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
css_token css_tokenizer::consume_number_sign(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
    tchar_t c2 = stream_.peek(1);
    css_token result;

    if (is_name_code_point(c1) || is_valid_escape(c1, c2)) {
        // FIXME: Determine hash type flag (either id or unrestricted);
        tstring value = consume_name(c0);
        result = css_token(kCSSTokenHash);
    } else {
        result = css_token(kCSSTokenDelim);
    }

    assert(result.type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
css_token css_tokenizer::consume_plus_sign(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
    tchar_t c2 = stream_.peek(1);
    css_token result;

    if (would_start_number(c0, c1, c2)) {
        result = consume_numeric(c0);
    } else {
        result = css_token(kCSSTokenDelim);
    }

    assert(result.type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
css_token css_tokenizer::consume_hyphen_minus(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
    tchar_t c2 = stream_.peek(1);
    css_token result;

    if (would_start_number(c0, c1, c2)) {
        result = consume_numeric(c0);
    } else if (c1 == _t('-') && c2 == _t('>')) {
        result = css_token(kCSSTokenCDC);
    } else if (would_start_identifier(c0, c1, c2)) {
        result = consume_ident(c0);
    } else {
        result = css_token(kCSSTokenDelim);
    }

    assert(result.type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
css_token css_tokenizer::consume_full_stop(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
    tchar_t c2 = stream_.peek(1);
    css_token result;

    if (would_start_number(c0, c1, c2)) {
        result = consume_numeric(c0);
    } else {
        result = css_token(kCSSTokenDelim);
    }

    assert(result.type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
css_token css_tokenizer::consume_less_than(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
    tchar_t c2 = stream_.peek(1);
    tchar_t c3 = stream_.peek(2);
    css_token result;

    if (c1 == _t('!') && c2 == _t('-') && c3 == _t('-')) {
        stream_.advance(3);
        result = css_token(kCSSTokenCDO);
    } else {
        result = css_token(kCSSTokenDelim);
    }

    assert(result.type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
css_token css_tokenizer::consume_at(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
    tchar_t c2 = stream_.peek(1);
    tchar_t c3 = stream_.peek(2);
    css_token result;

    if (would_start_identifier(c1, c2, c3)) {
        stream_.advance();
        tstring name = consume_name(c1);
        result = css_token(kCSSTokenAtKeyword);
    } else {
        result = css_token(kCSSTokenDelim);
    }

    assert(result.type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-token
css_token css_tokenizer::consume_backslash(tchar_t c0)
{
    tchar_t c1 = stream_.peek(0);
    css_token result;

    if (is_valid_escape(c0, c1)) {
        result = consume_ident(c0);
    } else {
        // FIXME: Parse error
        result = css_token(kCSSTokenDelim);
    }

    assert(result.type() != kCSSTokenNone);

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-comment
void css_tokenizer::consume_comment()
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
css_token css_tokenizer::consume_numeric(tchar_t first)
{
    return css_token(kCSSTokenNumber, consume_number(first));
}


// https://www.w3.org/TR/css-syntax-3/#consume-ident-like-token
css_token css_tokenizer::consume_ident(tchar_t first)
{
    css_token result(kCSSTokenIdent, consume_name(first));

    // TODO: Handle URLs

    return result;
}

// https://www.w3.org/TR/css-syntax-3/#consume-string-token
css_token css_tokenizer::consume_string(tchar_t ending)
{
    css_token token;
    tstring value;

    while (true) {
        tchar_t c = stream_.consume();
        if (c == ending) {
            token = css_token(kCSSTokenString, value);
            break;
        } else if (c == _t('\0')) {
            // FIXME: Indicate a parse error occurred.
            token = css_token(kCSSTokenString, value);
            break;
        } else if (is_newline(c)) {
            token = css_token(kCSSTokenBadString);
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

    assert(token.type() == kCSSTokenString || token.type() == kCSSTokenBadString);

    return token;
}

// https://www.w3.org/TR/css-syntax-3/#consume-escaped-code-point
css_token css_tokenizer::consume_url()
{
    return css_token(kCSSTokenURL);
}

// https://www.w3.org/TR/css-syntax-3/#consume-escaped-code-point
tchar_t css_tokenizer::consume_escape()
{
    tchar_t result = 0;
    tchar_t c = stream_.consume();

    if (is_hex_digit(c)) {
        // FIXME: Implement
    } else if (is_eof(c)) {
        // FIXME: Parse error
        // FIXME: Return replacement character
    } else {
        result = c;
    }

    return c;
}

// https://www.w3.org/TR/css-syntax-3/#consume-name
tstring css_tokenizer::consume_name(tchar_t first)
{
    tstring result;
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
css_number css_tokenizer::consume_number(tchar_t first)
{
    double value = 0.0;
    css_number_value_type type = kCSSIntegerValue;
    tstring repr;

    if (first == _t('+') || first == _t('-')) {
        repr.push_back(stream_.peek(0));
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

    return css_number(type, std::stod(repr));
}

css_token css_tokenizer::next()
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
            return css_token(kCSSTokenEOF);

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
            return css_token(kCSSTokenComma);

        case _t('-'):
            return consume_hyphen_minus(c);

        case _t('.'):
            return consume_full_stop(c);

        case _t(':'):
            return css_token(kCSSTokenColon);

        case _t(';'):
            return css_token(kCSSTokenColon);

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
            return css_token(kCSSTokenOpenRoundBracket);
        case _t(')'):
            return css_token(kCSSTokenCloseRoundBracket);

        case _t('['):
            return css_token(kCSSTokenOpenSquareBracket);
        case _t(']'):
            return css_token(kCSSTokenCloseSquareBracket);

        case _t('{'):
            return css_token(kCSSTokenOpenBrace);
        case _t('}'):
            return css_token(kCSSTokenCloseBrace);

        default:
            return css_token(kCSSTokenNone);
    }
}

} // namespace litehtml
