// Copyright (c) 2013, Yuri Kobets (tordex)
// Copyright (C) 2020-2021 Primate Labs Inc.
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

#include "litehtml/css/css_style.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <locale>
#include <sstream>

#include "litehtml/css/css_component_value.h"
#include "litehtml/css/css_regenerate.h"
#include "litehtml/html.h"

namespace litehtml {

CSSStyle::CSSStyle()
{
}

CSSStyle::~CSSStyle()
{
}

void CSSStyle::parse(const std::string& txt, const URL& baseurl)
{
    std::vector<std::string> properties;
    split_string(txt, properties, ";", "", "\"'");

    for (auto& property : properties) {
        parse_property(property, baseurl);
    }
}

void CSSStyle::parse_property(const std::string& txt, const URL& baseurl)
{
    std::string::size_type pos = txt.find_first_of(":");
    if (pos != std::string::npos) {
        std::string name = txt.substr(0, pos);
        std::string val = txt.substr(pos + 1);

        trim(name);
        lcase(name);
        trim(val);

        if (!name.empty() && !val.empty()) {
            string_vector vals;
            split_string(val, vals, "!");
            if (vals.size() == 1) {
                add_property(name.c_str(), val.c_str(), baseurl, false);
            } else if (vals.size() > 1) {
                trim(vals[0]);
                lcase(vals[1]);
                add_property(name.c_str(),
                    vals[0].c_str(),
                    baseurl,
                    vals[1] == "important");
            }
        }
    }
}

void CSSStyle::combine(const CSSStyle& other)
{
    for (auto& property : other.properties_) {
        // FIXME: This approach may only work for string values.
        // assert(property.second->is_string());
        add_parsed_property(property.first,
            property.second->string(),
            property.second->important());
    }
}

#if defined(ENABLE_JSON)

nlohmann::json CSSStyle::json() const
{
    nlohmann::json result;

    for (auto& property : properties_) {
        result[property.first] = property.second->json();
    }

    return result;
}

#endif


void CSSStyle::add_property(CSSProperty name,
    const char* val,
    const URL& url,
    bool important)
{
    if (!val) {
        return;
    }

    // Add baseurl for background image
    if (name == kCSSPropertyBackgroundImage) {
        add_parsed_property(name, val, important);
        if (!url.empty()) {
            add_parsed_property(kCSSPropertyBackgroundImageBaseurl, url.string(), important);
        }
    } else if (name == kCSSPropertyBorderSpacing) {
        // Parse border spacing properties
        string_vector tokens;
        split_string(val, tokens, " ");
        if (tokens.size() == 1) {
            add_parsed_property(kCSSPropertyLitehtmlBorderSpacingX,
                tokens[0].c_str(),
                important);
            add_parsed_property(kCSSPropertyLitehtmlBorderSpacingY,
                tokens[0].c_str(),
                important);
        } else if (tokens.size() == 2) {
            add_parsed_property(kCSSPropertyLitehtmlBorderSpacingX,
                tokens[0].c_str(),
                important);
            add_parsed_property(kCSSPropertyLitehtmlBorderSpacingY,
                tokens[1].c_str(),
                important);
        }
    } else if (name == kCSSPropertyBorder) {

        // Parse borders shorthand properties
        string_vector tokens;
        split_string(val, tokens, " ", "", "(");
        int idx;
        std::string str;
        for (string_vector::const_iterator tok = tokens.begin();
             tok != tokens.end();
             tok++) {
            idx = value_index(tok->c_str(), BORDER_STYLE_STRINGS, -1);
            if (idx >= 0) {
                add_property(kCSSPropertyBorderLeftStyle, tok->c_str(), url, important);
                add_property(kCSSPropertyBorderRightStyle, tok->c_str(), url, important);
                add_property(kCSSPropertyBorderTopStyle, tok->c_str(), url, important);
                add_property(kCSSPropertyBorderBottomStyle, tok->c_str(), url, important);
            } else {
                if (isdigit((*tok)[0]) || (*tok)[0] == '.' ||
                    value_in_list((*tok), "thin;medium;thick")) {
                    add_property(kCSSPropertyBorderLeftWidth,
                        tok->c_str(),
                        url,
                        important);
                    add_property(kCSSPropertyBorderRightWidth,
                        tok->c_str(),
                        url,
                        important);
                    add_property(kCSSPropertyBorderTopWidth,
                        tok->c_str(),
                        url,
                        important);
                    add_property(kCSSPropertyBorderBottomWidth,
                        tok->c_str(),
                        url,
                        important);
                } else {
                    add_property(kCSSPropertyBorderLeftColor,
                        tok->c_str(),
                        url,
                        important);
                    add_property(kCSSPropertyBorderRightColor,
                        tok->c_str(),
                        url,
                        important);
                    add_property(kCSSPropertyBorderTopColor,
                        tok->c_str(),
                        url,
                        important);
                    add_property(kCSSPropertyBorderBottomColor,
                        tok->c_str(),
                        url,
                        important);
                }
            }
        }
    } else if ((name == kCSSPropertyBorderLeft) ||
               (name == kCSSPropertyBorderRight) ||
               (name == kCSSPropertyBorderTop) ||
               (name == kCSSPropertyBorderBottom)) {
        string_vector tokens;
        split_string(val, tokens, " ", "", "(");
        int idx;
        std::string str;
        for (string_vector::const_iterator tok = tokens.begin();
             tok != tokens.end();
             tok++) {
            idx = value_index(tok->c_str(), BORDER_STYLE_STRINGS, -1);
            if (idx >= 0) {
                str = css_property_string(name);
                str += "-style";
                add_property(str.c_str(), tok->c_str(), url, important);
            } else {
                if (Color::is_color(tok->c_str())) {
                    str = css_property_string(name);
                    str += "-color";
                    add_property(str.c_str(), tok->c_str(), url, important);
                } else {
                    str = css_property_string(name);
                    str += "-width";
                    add_property(str.c_str(), tok->c_str(), url, important);
                }
            }
        }
    } else if (name == kCSSPropertyBorderBottomLeftRadius) {

        // Parse border radius shorthand properties

        string_vector tokens;
        split_string(val, tokens, " ");
        if (tokens.size() >= 2) {
            add_property(kCSSPropertyBorderBottomLeftRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomLeftRadiusY,
                tokens[1].c_str(),
                url,
                important);
        } else if (tokens.size() == 1) {
            add_property(kCSSPropertyBorderBottomLeftRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomLeftRadiusY,
                tokens[0].c_str(),
                url,
                important);
        }

    } else if (name == kCSSPropertyBorderBottomRightRadius) {
        string_vector tokens;
        split_string(val, tokens, " ");
        if (tokens.size() >= 2) {
            add_property(kCSSPropertyBorderBottomRightRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomRightRadiusY,
                tokens[1].c_str(),
                url,
                important);
        } else if (tokens.size() == 1) {
            add_property(kCSSPropertyBorderBottomRightRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomRightRadiusY,
                tokens[0].c_str(),
                url,
                important);
        }

    } else if (name == kCSSPropertyBorderTopRightRadius) {
        string_vector tokens;
        split_string(val, tokens, " ");
        if (tokens.size() >= 2) {
            add_property(kCSSPropertyBorderTopRightRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderTopRightRadiusY,
                tokens[1].c_str(),
                url,
                important);
        } else if (tokens.size() == 1) {
            add_property(kCSSPropertyBorderTopRightRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderTopRightRadiusY,
                tokens[0].c_str(),
                url,
                important);
        }

    } else if (name == kCSSPropertyBorderTopLeftRadius) {
        string_vector tokens;
        split_string(val, tokens, " ");
        if (tokens.size() >= 2) {
            add_property(kCSSPropertyBorderTopLeftRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderTopLeftRadiusY,
                tokens[1].c_str(),
                url,
                important);
        } else if (tokens.size() == 1) {
            add_property(kCSSPropertyBorderTopLeftRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderTopLeftRadiusY,
                tokens[0].c_str(),
                url,
                important);
        }

    } else if (name == kCSSPropertyBorderRadius) {

        // Parse border-radius shorthand properties

        string_vector tokens;
        split_string(val, tokens, "/");
        if (tokens.size() == 1) {
            add_property(kCSSPropertyBorderRadiusX, tokens[0].c_str(), url, important);
            add_property(kCSSPropertyBorderRadiusY, tokens[0].c_str(), url, important);
        } else if (tokens.size() >= 2) {
            add_property(kCSSPropertyBorderRadiusX, tokens[0].c_str(), url, important);
            add_property(kCSSPropertyBorderRadiusY, tokens[1].c_str(), url, important);
        }
    } else if (name == kCSSPropertyBorderRadiusX) {
        string_vector tokens;
        split_string(val, tokens, " ");
        if (tokens.size() == 1) {
            add_property(kCSSPropertyBorderTopLeftRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderTopRightRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomRightRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomLeftRadiusX,
                tokens[0].c_str(),
                url,
                important);
        } else if (tokens.size() == 2) {
            add_property(kCSSPropertyBorderTopLeftRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderTopRightRadiusX,
                tokens[1].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomRightRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomLeftRadiusX,
                tokens[1].c_str(),
                url,
                important);
        } else if (tokens.size() == 3) {
            add_property(kCSSPropertyBorderTopLeftRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderTopRightRadiusX,
                tokens[1].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomRightRadiusX,
                tokens[2].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomLeftRadiusX,
                tokens[1].c_str(),
                url,
                important);
        } else if (tokens.size() == 4) {
            add_property(kCSSPropertyBorderTopLeftRadiusX,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderTopRightRadiusX,
                tokens[1].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomRightRadiusX,
                tokens[2].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomLeftRadiusX,
                tokens[3].c_str(),
                url,
                important);
        }
    } else if (name ==  kCSSPropertyBorderRadiusY) {
        string_vector tokens;
        split_string(val, tokens, " ");
        if (tokens.size() == 1) {
            add_property(kCSSPropertyBorderTopLeftRadiusY,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderTopRightRadiusY,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomRightRadiusY,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomLeftRadiusY,
                tokens[0].c_str(),
                url,
                important);
        } else if (tokens.size() == 2) {
            add_property(kCSSPropertyBorderTopLeftRadiusY,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderTopRightRadiusY,
                tokens[1].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomRightRadiusY,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomLeftRadiusY,
                tokens[1].c_str(),
                url,
                important);
        } else if (tokens.size() == 3) {
            add_property(kCSSPropertyBorderTopLeftRadiusY,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderTopRightRadiusY,
                tokens[1].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomRightRadiusY,
                tokens[2].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomLeftRadiusY,
                tokens[1].c_str(),
                url,
                important);
        } else if (tokens.size() == 4) {
            add_property(kCSSPropertyBorderTopLeftRadiusY,
                tokens[0].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderTopRightRadiusY,
                tokens[1].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomRightRadiusY,
                tokens[2].c_str(),
                url,
                important);
            add_property(kCSSPropertyBorderBottomLeftRadiusY,
                tokens[3].c_str(),
                url,
                important);
        }
    } else if (name == kCSSPropertyListStyle) {

        // Parse list-style shorthand properties

        add_parsed_property(kCSSPropertyListStyleType, "disc", important);
        add_parsed_property(kCSSPropertyListStylePosition, "outside", important);
        add_parsed_property(kCSSPropertyListStyleImage, "", important);
        add_parsed_property(kCSSPropertyListStyleImageBaseurl, "", important);

        string_vector tokens;
        split_string(val, tokens, " ", "", "(");
        for (string_vector::iterator tok = tokens.begin(); tok != tokens.end();
             tok++) {
            int idx = value_index(tok->c_str(), LIST_STYLE_TYPE_STRINGS, -1);
            if (idx >= 0) {
                add_parsed_property(kCSSPropertyListStyleType, *tok, important);
            } else {
                idx = value_index(tok->c_str(), LIST_STYLE_POSITION_STRINGS, -1);
                if (idx >= 0) {
                    add_parsed_property(kCSSPropertyListStylePosition, *tok, important);
                } else if (!strncmp(val, "url", 3)) {
                    add_parsed_property(kCSSPropertyListStyleImage, *tok, important);
                    if (!url.empty()) {
                        add_parsed_property(kCSSPropertyListStyleImageBaseurl,
                            url.string(),
                            important);
                    }
                }
            }
        }
    } else if (name == kCSSPropertyListStyleImage) {

        // Add baseurl for background image

        add_parsed_property(name, val, important);
        if (!url.empty()) {
            add_parsed_property(kCSSPropertyListStyleImageBaseurl, url.string(), important);
        }
    } else if (name == kCSSPropertyBackground) {

        // Parse background shorthand properties

        parse_short_background(val, url, important);

    } else if ((name == kCSSPropertyMargin) || (name == kCSSPropertyPadding)) {

        // Parse margin and padding shorthand properties

        String hack = css_property_string(name);

        string_vector tokens;
        split_string(val, tokens, " ");
        if (tokens.size() >= 4) {
            add_parsed_property(hack + "-top", tokens[0], important);
            add_parsed_property(hack + "-right", tokens[1], important);
            add_parsed_property(hack + "-bottom", tokens[2], important);
            add_parsed_property(hack + "-left", tokens[3], important);
        } else if (tokens.size() == 3) {
            add_parsed_property(hack + "-top", tokens[0], important);
            add_parsed_property(hack + "-right", tokens[1], important);
            add_parsed_property(hack + "-left", tokens[1], important);
            add_parsed_property(hack + "-bottom", tokens[2], important);
        } else if (tokens.size() == 2) {
            add_parsed_property(hack + "-top", tokens[0], important);
            add_parsed_property(hack + "-bottom", tokens[0], important);
            add_parsed_property(hack + "-right", tokens[1], important);
            add_parsed_property(hack + "-left", tokens[1], important);
        } else if (tokens.size() == 1) {
            add_parsed_property(hack + "-top", tokens[0], important);
            add_parsed_property(hack + "-bottom", tokens[0], important);
            add_parsed_property(hack + "-right", tokens[0], important);
            add_parsed_property(hack + "-left", tokens[0], important);
        }
    } else if ((name == kCSSPropertyBorderLeft) ||
               (name == kCSSPropertyBorderRight) ||
               (name == kCSSPropertyBorderTop) ||
               (name == kCSSPropertyBorderBottom)) {

        // Parse border-* shorthand properties
        parse_short_border(name, val, important);
    } else if ((name == kCSSPropertyBorderWidth) ||
               (name == kCSSPropertyBorderStyle) ||
               (name == kCSSPropertyBorderColor)) {

        // Parse border-width/style/color shorthand properties
        string_vector nametokens;
        split_string(css_property_string(name), nametokens, "-");

        string_vector tokens;
        split_string(val, tokens, " ");
        if (tokens.size() >= 4) {
            add_parsed_property(nametokens[0] + "-top-" + nametokens[1],
                tokens[0],
                important);
            add_parsed_property(nametokens[0] + "-right-" + nametokens[1],
                tokens[1],
                important);
            add_parsed_property(nametokens[0] + "-bottom-" + nametokens[1],
                tokens[2],
                important);
            add_parsed_property(nametokens[0] + "-left-" + nametokens[1],
                tokens[3],
                important);
        } else if (tokens.size() == 3) {
            add_parsed_property(nametokens[0] + "-top-" + nametokens[1],
                tokens[0],
                important);
            add_parsed_property(nametokens[0] + "-right-" + nametokens[1],
                tokens[1],
                important);
            add_parsed_property(nametokens[0] + "-left-" + nametokens[1],
                tokens[1],
                important);
            add_parsed_property(nametokens[0] + "-bottom-" + nametokens[1],
                tokens[2],
                important);
        } else if (tokens.size() == 2) {
            add_parsed_property(nametokens[0] + "-top-" + nametokens[1],
                tokens[0],
                important);
            add_parsed_property(nametokens[0] + "-bottom-" + nametokens[1],
                tokens[0],
                important);
            add_parsed_property(nametokens[0] + "-right-" + nametokens[1],
                tokens[1],
                important);
            add_parsed_property(nametokens[0] + "-left-" + nametokens[1],
                tokens[1],
                important);
        } else if (tokens.size() == 1) {
            add_parsed_property(nametokens[0] + "-top-" + nametokens[1],
                tokens[0],
                important);
            add_parsed_property(nametokens[0] + "-bottom-" + nametokens[1],
                tokens[0],
                important);
            add_parsed_property(nametokens[0] + "-right-" + nametokens[1],
                tokens[0],
                important);
            add_parsed_property(nametokens[0] + "-left-" + nametokens[1],
                tokens[0],
                important);
        }
    } else if (name == kCSSPropertyFont) {

        // Parse font shorthand properties

        parse_short_font(val, important);
    } else {
        add_parsed_property(name, val, important);
    }
}

void CSSStyle::add_property(CSSDeclaration* declaration)
{
    std::ostringstream oss;

    // CSSStyle methods expect an unparsed string for the declaration value.
    // Build a declaration value string from the declaration value component
    // values.
    //
    // This is a horrible and should be fixed ASAP.

    String hack = css_regenerate(declaration->values());

    add_property(declaration->name().c_str(),
        hack.c_str(),
        URL(),
        declaration->important());
}

void CSSStyle::parse_short_border(CSSProperty prefix, const std::string& val, bool important)
{
    String hack = css_property_string(prefix);

    string_vector tokens;
    split_string(val, tokens, " ", "", "(");
    if (tokens.size() >= 3) {
        add_parsed_property(hack + "-width", tokens[0], important);
        add_parsed_property(hack + "-style", tokens[1], important);
        add_parsed_property(hack + "-color", tokens[2], important);
    } else if (tokens.size() == 2) {
        if (iswdigit(tokens[0][0]) ||
            value_index(val.c_str(), border_width_strings) >= 0) {
            add_parsed_property(hack + "-width", tokens[0], important);
            add_parsed_property(hack + "-style", tokens[1], important);
        } else {
            add_parsed_property(hack + "-style", tokens[0], important);
            add_parsed_property(hack + "-color", tokens[1], important);
        }
    }
}

void CSSStyle::parse_short_background(const std::string& val,
    const URL& baseurl,
    bool important)
{
    add_parsed_property(kCSSPropertyBackgroundColor, "transparent", important);
    add_parsed_property(kCSSPropertyBackgroundImage, "", important);
    add_parsed_property(kCSSPropertyBackgroundImageBaseurl, "", important);
    add_parsed_property(kCSSPropertyBackgroundRepeat, "repeat", important);
    add_parsed_property(kCSSPropertyBackgroundOrigin, "padding-box", important);
    add_parsed_property(kCSSPropertyBackgroundClip, "border-box", important);
    add_parsed_property(kCSSPropertyBackgroundAttachment, "scroll", important);

    if (val == "none") {
        return;
    }

    string_vector tokens;
    split_string(val, tokens, " ", "", "(");
    bool origin_found = false;
    for (string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++) {
        if (tok->substr(0, 3) == "url") {
            add_parsed_property(kCSSPropertyBackgroundImage, *tok, important);
            if (!baseurl.empty()) {
                add_parsed_property(kCSSPropertyBackgroundImageBaseurl,
                    baseurl.string(),
                    important);
            }

        } else if (value_in_list(tok->c_str(), BACKGROUND_REPEAT_STRINGS)) {
            add_parsed_property(kCSSPropertyBackgroundRepeat, *tok, important);
        } else if (value_in_list(tok->c_str(), BACKGROUND_ATTACHMENT_STRINGS)) {
            add_parsed_property(kCSSPropertyBackgroundAttachment, *tok, important);
        } else if (value_in_list(tok->c_str(), BACKGROUND_BOX_STRINGS)) {
            if (!origin_found) {
                add_parsed_property(kCSSPropertyBackgroundOrigin, *tok, important);
                origin_found = true;
            } else {
                add_parsed_property(kCSSPropertyBackgroundClip, *tok, important);
            }
        } else if (value_in_list(tok->c_str(), "left;right;top;bottom;center") ||
                   iswdigit((*tok)[0]) || (*tok)[0] == '-' ||
                   (*tok)[0] == '.' || (*tok)[0] == '+') {
            if (properties_.find(kCSSPropertyBackgroundPosition) != properties_.end()) {
                std::string new_value = properties_[kCSSPropertyBackgroundPosition]->string() + " " + *tok;
                assert(properties_[kCSSPropertyBackgroundPosition]->is_string());
                properties_[kCSSPropertyBackgroundPosition]->string(new_value);
            } else {
                add_parsed_property(kCSSPropertyBackgroundPosition, *tok, important);
            }
        } else if (Color::is_color(tok->c_str())) {
            add_parsed_property(kCSSPropertyBackgroundColor, *tok, important);
        }
    }
}

void CSSStyle::parse_short_font(const std::string& val, bool important)
{
    add_parsed_property(kCSSPropertyFontStyle, "normal", important);
    add_parsed_property(kCSSPropertyFontVariant, "normal", important);
    add_parsed_property(kCSSPropertyFontWeight, "normal", important);
    add_parsed_property(kCSSPropertyFontSize, "medium", important);
    add_parsed_property(kCSSPropertyLineHeight, "normal", important);

    string_vector tokens;
    split_string(val, tokens, " ", "", "\"");

    int idx = 0;
    bool was_normal = false;
    bool is_family = false;
    std::string font_family;
    for (string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++) {
        idx = value_index(tok->c_str(), font_style_strings);
        if (!is_family) {
            if (idx >= 0) {
                if (idx == 0 && !was_normal) {
                    add_parsed_property(kCSSPropertyFontWeight, *tok, important);
                    add_parsed_property(kCSSPropertyFontVariant, *tok, important);
                    add_parsed_property(kCSSPropertyFontStyle, *tok, important);
                } else {
                    add_parsed_property(kCSSPropertyFontStyle, *tok, important);
                }
            } else {
                if (value_in_list(tok->c_str(), FONT_WEIGHT_STRINGS)) {
                    add_parsed_property(kCSSPropertyFontWeight, *tok, important);
                } else {
                    if (value_in_list(tok->c_str(), font_variant_strings)) {
                        add_parsed_property(kCSSPropertyFontVariant, *tok, important);
                    } else if (iswdigit((*tok)[0])) {
                        string_vector szlh;
                        split_string(*tok, szlh, "/");

                        if (szlh.size() == 1) {
                            add_parsed_property(kCSSPropertyFontSize, szlh[0], important);
                        } else if (szlh.size() >= 2) {
                            add_parsed_property(kCSSPropertyFontSize, szlh[0], important);
                            add_parsed_property(kCSSPropertyLineHeight, szlh[1], important);
                        }
                    } else {
                        is_family = true;
                        font_family += *tok;
                    }
                }
            }
        } else {
            font_family += *tok;
        }
    }
    add_parsed_property(kCSSPropertyFontFamily, font_family, important);
}

void CSSStyle::add_parsed_property(CSSProperty name, const std::string& str, bool important)
{
    // FIXME: How do we validate property values?
#if 0
    bool is_valid = true;
    auto vals = m_valid_values.find(name);
    if (vals != m_valid_values.end()) {
        if (!value_in_list(val, vals->second)) {
            is_valid = false;
        }
    }
#endif

    // FIXME: How do we handle values that aren't valid?

    auto property = properties_.find(name);
    if (property != properties_.end()) {
        const CSSValue* value = property->second.get();
        if (!value->important() || (important && value->important())) {
            properties_[name].reset(CSSValue::factory(name, str, important));
        }
    } else {
        properties_[name].reset(CSSValue::factory(name, str, important));
    }
}

void CSSStyle::remove_property(CSSProperty name, bool important)
{
    auto property = properties_.find(name);
    if (property != properties_.end()) {
        const CSSValue* value = property->second.get();
        if (!value->important() || (important && value->important())) {
            properties_.erase(property);
        }
    }
}

} // namespace litehtml
