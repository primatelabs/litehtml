// Copyright (c) 2013, Yuri Kobets (tordex)
// Copyright (c) 2020-2021 Primate Labs Inc.
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

#include "litehtml/media_query.h"

#include "litehtml/document.h"
#include "litehtml/document_container.h"
#include "litehtml/html.h"

namespace litehtml {

MediaQuery::ptr MediaQuery::create_from_string(const std::string& str,
    const Document* doc)
{
    MediaQuery::ptr query = std::make_shared<MediaQuery>();

    string_vector tokens;
    split_string(str, tokens, " \t\r\n", "", "(");

    for (string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++) {
        if ((*tok) == "not") {
            query->not_ = true;
        } else if (tok->at(0) == '(') {
            tok->erase(0, 1);
            if (tok->at(tok->length() - 1) == ')') {
                tok->erase(tok->length() - 1, 1);
            }
            MediaQueryExpression expr;
            string_vector expr_tokens;
            split_string((*tok), expr_tokens, ":");
            if (!expr_tokens.empty()) {
                trim(expr_tokens[0]);
                expr.feature = (MediaFeature)value_index(expr_tokens[0],
                    MEDIA_FEATURE_STRINGS,
                    kMediaFeatureNone);
                if (expr.feature != kMediaFeatureNone) {
                    if (expr_tokens.size() == 1) {
                        expr.check_as_bool = true;
                    } else {
                        trim(expr_tokens[1]);
                        expr.check_as_bool = false;
                        if (expr.feature == kMediaFeatureOrientation) {
                            expr.val = value_index(expr_tokens[1],
                                MEDIA_ORIENTATION_STRINGS,
                                kMediaOrientationLandscape);
                        } else {
                            std::string::size_type slash_pos =
                                expr_tokens[1].find('/');
                            if (slash_pos != std::string::npos) {
                                std::string val1 = expr_tokens[1].substr(0, slash_pos);
                                std::string val2 =
                                    expr_tokens[1].substr(slash_pos + 1);
                                trim(val1);
                                trim(val2);
                                expr.val = atoi(val1.c_str());
                                expr.val2 = atoi(val2.c_str());
                            } else {
                                CSSLength length;
                                length.parse_length_string(expr_tokens[1]);
                                if (length.units() == kCSSUnitsDpcm) {
                                    expr.val = (int)(length.val() * 2.54);
                                } else if (length.units() == kCSSUnitsDpi) {
                                    expr.val = (int)(length.val() * 2.54);
                                } else {
                                    if (doc) {
                                        doc->cvt_units(length,
                                            doc->container()->get_default_font_size());
                                    }
                                    expr.val = (int)length.val();
                                }
                            }
                        }
                    }
                    query->expressions_.push_back(expr);
                }
            }
        } else {
            query->media_type_ =
                (MediaType)value_index((*tok), MEDIA_TYPE_STRINGS, kMediaTypeAll);
        }
    }

    return query;
}

bool MediaQuery::check(const MediaFeatures& features) const
{
    bool result = false;
    if (media_type_ == kMediaTypeAll || media_type_ == features.type) {
        result = true;
        for (auto& expression : expressions_) {
            if (!expression.check(features)) {
                result = false;
                break;
            }
        }
    }

    if (not_) {
        result = !result;
    }

    return result;
}

} // namespace litehtml
