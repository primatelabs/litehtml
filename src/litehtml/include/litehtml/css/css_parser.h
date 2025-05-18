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

#ifndef LITEHTML_CSS_PARSER_H__
#define LITEHTML_CSS_PARSER_H__

#include "litehtml/css/css_block.h"
#include "litehtml/css/css_component_value.h"
#include "litehtml/css/css_declaration.h"
#include "litehtml/css/css_function.h"
#include "litehtml/css/css_prelude.h"
#include "litehtml/css/css_range.h"
#include "litehtml/css/css_style.h"
#include "litehtml/css/css_stylesheet.h"
#include "litehtml/css/css_tokenizer.h"
#include "litehtml/types.h"

namespace litehtml {

class CSSParser {
protected:
    CSSTokenizer tokenizer_;

    std::vector<CSSRule*> consume_rules(CSSTokenRange& range, bool top_level);

    CSSRule* consume_at_rule(CSSTokenRange& range);

    CSSRule* consume_qualified_rule(CSSTokenRange& range);

    CSSComponentValue* consume_component_value(CSSTokenRange& range);

    CSSBlock* consume_block(CSSTokenRange& range,
        CSSToken* starting_token);

    CSSFunction* consume_function(CSSTokenRange& range,
        CSSToken* starting_token);

    CSSDeclaration* consume_declaration(CSSComponentValueRange& range);

    CSSStyle* consume_declarations(CSSComponentValueRange& range);

public:
    explicit CSSParser(const std::string& input);

    CSSStylesheet* parse_stylesheet();

    void parse_stylesheet(CSSStylesheet* stylesheet);
};

} // namespace litehtml

#endif // LITEHTML_CSS_PARSER_H__
