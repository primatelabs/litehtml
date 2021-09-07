// Copyright (C) 2020-2021 Primate Labs Inc. All rights reserved.
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

#include "litehtml/css_parser.h"

#include <iostream>

#include "litehtml/css_token_range.h"

namespace {


} // namespace

namespace litehtml {

css_parser::css_parser(const tstring& input)
: tokenizer_(input)
{
}

// https://www.w3.org/TR/css-syntax-3/#consume-list-of-rules
void css_parser::consume_rules(css_token_range& range, bool top_level)
{
    while (range.peek().type() != kCSSTokenEOF) {
        const css_token& token = range.consume();
        css_token_type type = token.type();

        std::cout << "consume_rules " << css_token_type_string(type) << std::endl;

        switch (type) {
            case kCSSTokenWhitespace:
                break;

            case kCSSTokenCDO:
            case kCSSTokenCDC:
                // If the top-level flag is set, do nothing.  Otherwise,
                // reconsume the current input token and consume a qualified
                // rule.  If anything is returned, append it to the list
                // of rules.

                if (!top_level) {
                    range.reconsume();
                    consume_qualified_rule(range);
                }
                break;

            case kCSSTokenAtKeyword:
                assert(false);
                break;

            default:
                range.reconsume();
                consume_qualified_rule(range);
                break;
        }
    }

    // Return a list of rules.
}

// https://www.w3.org/TR/css-syntax-3/#consume-at-rule
void css_parser::consume_at_rule(css_token_range& range)
{
    // TODO: Implement support for @import and @media (the only two at-rules
    // litehtml currently supports).
    assert(false);
}

// https://www.w3.org/TR/css-syntax-3/#consume-qualified-rule
void css_parser::consume_qualified_rule(css_token_range& range)
{
    while (range.peek().type() != kCSSTokenSemicolon) {
        const css_token& token = range.consume();
        css_token_type type = token.type();

        std::cout << "consume_qualified_rule " << css_token_type_string(type) << std::endl;

        switch (type) {
            case kCSSTokenEOF:
                // error
                assert(false);
                break;

            case kCSSTokenOpenBrace:
                consume_block(range, token);
                // TODO: Return the block
                return;

            default:
                range.reconsume();
                consume_component_value(range);
                // TODO: Return the component value
                return;
        }
    }
}

// https://www.w3.org/TR/css-syntax-3/#consume-component-value
void css_parser::consume_component_value(css_token_range& range)
{
    const css_token& token = range.consume();
    css_token_type type = token.type();

    std::cout << "consume_component_value " << css_token_type_string(type) << std::endl;

    switch (type) {
        case kCSSTokenOpenBrace:
        case kCSSTokenOpenSquareBracket:
        case kCSSTokenOpenRoundBracket:
            consume_block(range, token);
            break;

        case kCSSTokenFunction:
            consume_function(range);
            break;

        default:
            // return current token (how?!)
            // assert(false);
            break;
    }
}

// https://www.w3.org/TR/css-syntax-3/#consume-simple-block
void css_parser::consume_block(css_token_range& range, const css_token& starting_token)
{
    css_token_type ending_token_type = kCSSTokenNone;
    switch (starting_token.type()) {
        case kCSSTokenOpenBrace:
            ending_token_type = kCSSTokenCloseBrace;
            break;

        case kCSSTokenOpenSquareBracket:
            ending_token_type = kCSSTokenCloseSquareBracket;
            break;

        case kCSSTokenOpenRoundBracket:
            ending_token_type = kCSSTokenCloseRoundBracket;
            break;

        default:
            break;
    }

    while (true) {
        const css_token& token = range.consume();
        css_token_type type = token.type();

        std::cout << "consume_block " << css_token_type_string(type) << std::endl;

        if (type == ending_token_type) {
            break;
        } else if (type == kCSSTokenEOF) {
            // TODO: Raise a parse error before returning the block.
            assert(false);
            break;
        } else {
            range.reconsume();
            consume_component_value(range);
        }
    }
}

// https://www.w3.org/TR/css-syntax-3/#consume-function
void css_parser::consume_function(css_token_range& range)
{
    while (true) {
        const css_token& token = range.consume();
        css_token_type type = token.type();

        std::cout << "consume_function " << css_token_type_string(type) << std::endl;

        if (type == kCSSTokenCloseRoundBracket) {
            break;
        } else if (type == kCSSTokenEOF) {
            // TODO: Raise a parse error before returning the block.
            assert(false);
            break;
        } else {
            range.reconsume();
            consume_component_value(range);
        }
    }
}



// https://www.w3.org/TR/css-syntax-3/#parse-stylesheet
css_stylesheet css_parser::parse_stylesheet()
{
    css_token_range range(tokenizer_.tokens());
    consume_rules(range, true);

    return css_stylesheet();
}

} // namespace litehtml
