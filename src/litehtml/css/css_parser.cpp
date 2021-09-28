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

#include "litehtml/css/css_parser.h"

#include <assert.h>

#include <iostream>

#include "litehtml/css/css_token_range.h"

namespace {


} // namespace

namespace litehtml {

css_parser::css_parser(const tstring& input)
: tokenizer_(input)
{
}

// https://www.w3.org/TR/css-syntax-3/#consume-list-of-rules
std::vector<css_rule> css_parser::consume_rules(css_token_range& range,
    bool top_level)
{
    std::vector<css_rule> rules;

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
                    css_rule rule = consume_qualified_rule(range);
                    rules.push_back(rule);
                }
                break;

            case kCSSTokenAtKeyword:
                assert(false);
                break;

            default: {
                range.reconsume();
                css_rule rule = consume_qualified_rule(range);
                rules.push_back(rule);
                break;
            }
        }
    }

    return rules;
}

// https://www.w3.org/TR/css-syntax-3/#consume-at-rule
void css_parser::consume_at_rule(css_token_range& range)
{
    // TODO: Implement support for @import and @media (the only two at-rules
    // litehtml currently supports).
    assert(false);
}

// https://www.w3.org/TR/css-syntax-3/#consume-qualified-rule
css_rule css_parser::consume_qualified_rule(css_token_range& range)
{
    css_rule rule;

    while (true) {
        const css_token& token = range.consume();
        css_token_type type = token.type();

        if (type == kCSSTokenEOF) {
            // Parse error.  Return nothing.
            // TODO: Error handling.
            assert(false);
            break;
        } else if (type == kCSSTokenOpenBrace) {
            // Consume a simple block and assign it to the qualified rule's
            // block.  Return the qualified rule.
            rule.block_ = consume_block(range, token);
            break;
        } else {
            // Reconsume the current input token.  Consume a component value.
            // Append the returned value to the qualified rule’s prelude.
            range.reconsume();
            rule.prelude_.values_.push_back(consume_component_value(range));
        }
    }

    return rule;
}

// https://www.w3.org/TR/css-syntax-3/#consume-component-value
css_component_value css_parser::consume_component_value(css_token_range& range)
{
    const css_token& token = range.consume();
    css_token_type type = token.type();

    std::cout << "consume_component_value " << css_token_type_string(type)
              << std::endl;

    css_component_value value;

    switch (type) {
        case kCSSTokenOpenBrace:
        case kCSSTokenOpenSquareBracket:
        case kCSSTokenOpenRoundBracket:
            value.block_ = consume_block(range, token);
            value.type_ = kCSSComponentValueBlock;
            break;

        case kCSSTokenFunction:
            value.function_ = consume_function(range);
            value.type_ = kCSSComponentValueFunction;
            break;

        default:
            value.token_ = token;
            value.type_ = kCSSComponentValueToken;
            break;
    }

    return value;
}

// https://www.w3.org/TR/css-syntax-3/#consume-simple-block
css_block css_parser::consume_block(css_token_range& range,
    const css_token& starting_token)
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

    return css_block();
}

// https://www.w3.org/TR/css-syntax-3/#consume-function
css_function css_parser::consume_function(css_token_range& range)
{
    while (true) {
        const css_token& token = range.consume();
        css_token_type type = token.type();

        std::cout << "consume_function " << css_token_type_string(type)
                  << std::endl;

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

    return css_function();
}

// https://www.w3.org/TR/css-syntax-3/#parse-stylesheet
css_stylesheet css_parser::parse_stylesheet()
{
    css_stylesheet stylesheet;
    css_token_range range(tokenizer_.tokens());

    stylesheet.rules_ = consume_rules(range, true);

    return stylesheet;
}

} // namespace litehtml
