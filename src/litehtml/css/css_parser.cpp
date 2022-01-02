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

#include "litehtml/css/css_range.h"
#include "litehtml/css/css_regenerate.h"
#include "litehtml/html.h"
#include "litehtml/logging.h"

namespace {


} // namespace

namespace litehtml {

CSSParser::CSSParser(const tstring& input)
: tokenizer_(input)
{
}

// https://www.w3.org/TR/css-syntax-3/#consume-list-of-rules
std::vector<CSSRule*> CSSParser::consume_rules(CSSTokenRange& range,
    bool top_level)
{
    std::vector<CSSRule*> rules;

    while (range.peek()->type() != kCSSTokenEOF) {
        CSSToken* token = range.consume();
        CSSTokenType type = token->type();

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
                    CSSRule* rule = consume_qualified_rule(range);
                    rules.push_back(rule);
                }
                break;

            case kCSSTokenAtKeyword: {
                range.reconsume();
                CSSRule* rule = consume_at_rule(range);
                if (rule) {
                    rules.push_back(rule);
                }
                break;
            }

            default: {
                range.reconsume();
                CSSRule* rule = consume_qualified_rule(range);
                if (rule) {
                    rules.push_back(rule);
                }
                break;
            }
        }
    }

    return rules;
}

// https://www.w3.org/TR/css-syntax-3/#consume-at-rule
CSSRule* CSSParser::consume_at_rule(CSSTokenRange& range)
{
    // TODO: Implement support for @import and @media (the only two at-rules
    // litehtml currently supports).
    // assert(false);

    CSSRule* rule = new CSSRule();

    CSSToken* at_token = range.consume();
    assert(at_token->type() == kCSSTokenAtKeyword);
    rule->name(at_token->value());

    while (true) {
        CSSToken* token = range.consume();
        CSSTokenType type = token->type();

        if (type == kCSSTokenSemicolon) {
            // Return the at-rule.
            break;
        } else if (type == kCSSTokenEOF) {
            // This is a parse error. Return the at-rule.
            // FIXME: Log parse error.
            break;
        } else if (type == kCSSTokenOpenBrace) {
            // Consume a simple block and assign it to the at-rule’s block.
            // Return the at-rule.
            rule->block(consume_block(range, token));
            break;
        } else {
            // Reconsume the current input token.  Consume a component value.
            // Append the returned value to the at-rule’s prelude.
            range.reconsume();
            rule->prelude()->append(consume_component_value(range));
        }
    }

    return rule;
}

// https://www.w3.org/TR/css-syntax-3/#consume-qualified-rule
CSSRule* CSSParser::consume_qualified_rule(CSSTokenRange& range)
{
    CSSRule* rule = new CSSRule();

    while (true) {
        CSSToken* token = range.consume();
        CSSTokenType type = token->type();

        if (type == kCSSTokenEOF) {
            // Parse error.  Return nothing.
            // FIXME: Implement error handling (or reporting).
            delete rule;
            rule = nullptr;
            break;

        } else if (type == kCSSTokenOpenBrace) {
            // Consume a simple block and assign it to the qualified rule's
            // block.  Return the qualified rule.
            rule->block(consume_block(range, token));
            break;
        } else {
            // Reconsume the current input token.  Consume a component value.
            // Append the returned value to the qualified rule’s prelude.
            range.reconsume();
            rule->prelude()->append(consume_component_value(range));
        }
    }

    return rule;
}

// https://www.w3.org/TR/css-syntax-3/#consume-component-value
CSSComponentValue* CSSParser::consume_component_value(CSSTokenRange& range)
{
    CSSToken* token = range.consume();
    CSSTokenType type = token->type();

    CSSComponentValue* value = nullptr;

    switch (type) {
        case kCSSTokenOpenBrace:
        case kCSSTokenOpenSquareBracket:
        case kCSSTokenOpenRoundBracket: {
            CSSBlock* block = consume_block(range, token);
            if (block) {
                value = new CSSComponentValue(block);
            }
            break;
        }

        case kCSSTokenFunction: {
            CSSFunction* function = consume_function(range);
            if (function) {
                value = new CSSComponentValue(function);
            }
            break;
        }

        default: {
            value = new CSSComponentValue(token);
            break;
        }
    }

    return value;
}

// https://www.w3.org/TR/css-syntax-3/#consume-simple-block
CSSBlock* CSSParser::consume_block(CSSTokenRange& range,
    CSSToken* starting_token)
{
    CSSTokenType ending_token_type = kCSSTokenNone;
    switch (starting_token->type()) {
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

    CSSBlock* block = new CSSBlock();

    while (true) {
        CSSToken* token = range.consume();
        CSSTokenType type = token->type();

        if (type == ending_token_type) {
            break;
        } else if (type == kCSSTokenEOF) {
            // TODO: Raise a parse error before returning the block.
            std::cerr << "parser error" << std::endl;
            break;
        } else {
            range.reconsume();
            block->values_.push_back(consume_component_value(range));
        }
    }

    return block;
}

// https://www.w3.org/TR/css-syntax-3/#consume-function
CSSFunction* CSSParser::consume_function(CSSTokenRange& range)
{
    while (true) {
        CSSToken* token = range.consume();
        CSSTokenType type = token->type();

        if (type == kCSSTokenCloseRoundBracket) {
            break;
        } else if (type == kCSSTokenEOF) {
            // FIXME: Implement error handling (or reporting).
            break;
        } else {
            range.reconsume();
            consume_component_value(range);
        }
    }

    return new CSSFunction();
}

// https://www.w3.org/TR/css-syntax-3/#consume-declaration
CSSDeclaration* CSSParser::consume_declaration(CSSComponentValueRange& range)
{
    // Consume the next input token. Create a new declaration with its name
    // set to the value of the current input token and its value initially
    // set to the empty list.
    const CSSComponentValue* name = range.consume();

    // Verify the token is an identifier token.  It is an error if
    // consume_declaration() is called when the first token is not an
    // identifier token.
    assert(name->type() == kCSSTokenIdent);

    // 1. While the next input token is a <whitespace-token>, consume the next
    // input token.
    while (range.peek()->type() == kCSSTokenWhitespace) {
        range.consume();
    }

    // 2. If the next input token is anything other than a <colon-token>, this
    // is a parse error.  Return nothing.  Otherwise, consume the next input
    // token.
    if (range.peek()->type() != kCSSTokenColon) {
        // TODO: Log an error message for the parse error.
        return nullptr;
    }
    range.consume();

    // 3. While the next input token is a <whitespace-token>, consume the next
    // input token.
    while (range.peek()->type() == kCSSTokenWhitespace) {
        range.consume();
    }

    // 4. As long as the next input token is anything other than an
    // <EOF-token>, consume a component value and append it to the
    // declaration’s value.
    std::vector<CSSComponentValue*> values;
    while (true) {
        CSSComponentValue* value = range.consume();
        if (value->type() == kCSSTokenEOF) {
            break;
        }
        values.push_back(value);
    }

    // 6. While the last token in the declaration’s value is a
    // <whitespace-token>, remove that token.
    while (!values.empty() && values.back()->type() == kCSSTokenWhitespace) {
        values.pop_back();
    }

    // 5. If the last two non-<whitespace-token>s in the declaration’s value
    // are a <delim-token> with the value "!" followed by an <ident-token>
    // with a value that is an ASCII case-insensitive match for "important",
    // remove them from the declaration’s value and set the declaration’s
    // important flag to true.
    bool important = false;
    if (values.size() >= 2) {
        CSSComponentValue* delim = values[values.size() - 2];
        CSSComponentValue* ident = values[values.size() - 1];

        if (delim->type() == kCSSTokenDelim && delim->token()->value() == _t("!")) {
            if (ident->type() == kCSSTokenIdent && ident->token()->value() == _t("important")) {
                important = true;
                values.resize(values.size() - 1);
            }
        }
    }

    // 6. While the last token in the declaration’s value is a
    // <whitespace-token>, remove that token.  Note that this is done twice
    // (before and after the !important check) as removing !important from
    // the value can result in a value with trailing whitespace.
    while (!values.empty() && values.back()->type() == kCSSTokenWhitespace) {
        values.pop_back();
    }

    CSSDeclaration* declaration = nullptr;
    if (!values.empty()) {
        declaration = new CSSDeclaration(name->token()->value(), values, important);
    }
    return declaration;
}


// https://www.w3.org/TR/css-syntax-3/#consume-a-list-of-declarations
CSSStyle* CSSParser::consume_declarations(CSSComponentValueRange& range)
{
    CSSStyle* style = new CSSStyle();

    while (true) {
        const CSSComponentValue* value = range.consume();
        CSSTokenType type = value->type();

        if (type == kCSSTokenWhitespace || type == kCSSTokenSemicolon) {
            // Do nothing.
        } else if (type == kCSSTokenEOF) {
            // Return the list of declarations.
            break;

        } else if (type == kCSSTokenAtKeyword) {
            // Reconsume the current input token.  Consume an at-rule.
            // Append the returned rule to the list of declarations.
            assert(false);
            break;

        } else if (type == kCSSTokenIdent) {
            // Initialize a temporary list initially filled with the
            // current input token. As long as the next input token is
            // anything other than a <semicolon-token> or <EOF-token>,
            // consume a component value and append it to the temporary
            // list. Consume a declaration from the temporary list. If
            // anything was returned, append it to the list of
            // declarations.

            range.reconsume();
            CSSComponentValue** begin = range.position();
            while (true) {
                CSSComponentValue** end = range.position();
                const CSSComponentValue* tmp = range.consume();
                if (tmp->type() == kCSSTokenSemicolon || tmp->type() == kCSSTokenEOF) {
                    // consume a component value and append it to the list.
                    CSSComponentValueRange declaration_range(begin, end);
                    std::unique_ptr<CSSDeclaration> declaration(consume_declaration(declaration_range));
                    if (declaration) {
                        style->add_property(declaration.get());
                    }
                    break;
                }
            }

        } else {
            // This is a parse error. Reconsume the current input token.
            // As long as the next input token is anything other than a
            // <semicolon-token> or <EOF-token>, consume a component value
            // and throw away the returned value.

            // FIXME: Log parser error

        }
    }

    return style;
}

std::vector<String> consume_selectors(CSSComponentValueRange& range)
{
    std::vector<const CSSComponentValue*> tmp;
    std::vector<String> result;

    while (true) {
        const CSSComponentValue* value = range.consume();
        CSSTokenType type = value->type();

        if (type == kCSSTokenEOF || type == kCSSTokenComma) {
            if (!tmp.empty()) {
                String name = css_regenerate(tmp);
                tmp.clear();
                if (!name.empty()) {
                    result.push_back(name);
                }
            }

            if (type == kCSSTokenEOF) {
                break;
            }
        } else {
            tmp.push_back(value);
        }
    }
    return result;
}

// https://www.w3.org/TR/css-syntax-3/#parse-stylesheet
CSSStylesheet* CSSParser::parse_stylesheet()
{
    CSSStylesheet* stylesheet = new CSSStylesheet();
    parse_stylesheet(stylesheet);
    return stylesheet;
}


// https://www.w3.org/TR/css-syntax-3/#parse-stylesheet
void CSSParser::parse_stylesheet(CSSStylesheet* stylesheet)
{
    CSSTokenRange range(tokenizer_.tokens());

    std::vector<CSSRule*> rules = consume_rules(range, true);

    for (auto rule : rules) {

        // at rule has a name, qualified rule does not
        if (!rule->name().empty()) {
        } else {
            CSSComponentValueRange block_range(rule->block()->values_);

            CSSStyle::ptr style(consume_declarations(block_range));

            CSSComponentValueRange prelude_range(rule->prelude()->values_);

            std::vector<String> selector_strs = consume_selectors(prelude_range);
            for (auto selector_str : selector_strs) {
                assert(!selector_str.empty());

                CSSSelector::ptr selector = std::make_shared<CSSSelector>(nullptr);
                selector->m_style = style;
                trim(selector_str);
                if (selector->parse(selector_str)) {
                    selector->calc_specificity();
                    stylesheet->add_selector(selector);
                }
            }
        }

        //for (auto value : rule->prelude_->values()) {
        //    std::cout << css_token_type_string(value->type()) << std::endl;
        //}
    }

    for (auto rule : rules) {
        delete rule;
    }
}

} // namespace litehtml
