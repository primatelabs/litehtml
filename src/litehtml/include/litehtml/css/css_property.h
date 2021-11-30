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
//    * Neither the name of the copyright holders nor the names of their
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

#ifndef LITEHTML_CSS_CSS_PROPERTY_H__
#define LITEHTML_CSS_CSS_PROPERTY_H__

#include <vector>

#include "litehtml/os_types.h"

namespace litehtml {

class CSSValue;

enum CSSValueType : int;

enum CSSProperty : int {
    kCSSPropertyNone,
    kCSSPropertyLitehtmlBorderSpacingX,
    kCSSPropertyLitehtmlBorderSpacingY,
    kCSSPropertyBackground,
    kCSSPropertyBackgroundAttachment,
    kCSSPropertyBackgroundClip,
    kCSSPropertyBackgroundColor,
    kCSSPropertyBackgroundImage,
    kCSSPropertyBackgroundImageBaseurl,
    kCSSPropertyBackgroundOrigin,
    kCSSPropertyBackgroundPosition,
    kCSSPropertyBackgroundRepeat,
    kCSSPropertyBackgroundSize,
    kCSSPropertyBorder,
    kCSSPropertyBorderBottom,
    kCSSPropertyBorderBottomColor,
    kCSSPropertyBorderBottomLeftRadius,
    kCSSPropertyBorderBottomLeftRadiusX,
    kCSSPropertyBorderBottomLeftRadiusY,
    kCSSPropertyBorderBottomRightRadius,
    kCSSPropertyBorderBottomRightRadiusX,
    kCSSPropertyBorderBottomRightRadiusY,
    kCSSPropertyBorderBottomStyle,
    kCSSPropertyBorderBottomWidth,
    kCSSPropertyBorderCollapse,
    kCSSPropertyBorderColor,
    kCSSPropertyBorderLeft,
    kCSSPropertyBorderLeftColor,
    kCSSPropertyBorderLeftStyle,
    kCSSPropertyBorderLeftWidth,
    kCSSPropertyBorderRadius,
    kCSSPropertyBorderRadiusX,
    kCSSPropertyBorderRadiusY,
    kCSSPropertyBorderRight,
    kCSSPropertyBorderRightColor,
    kCSSPropertyBorderRightStyle,
    kCSSPropertyBorderRightWidth,
    kCSSPropertyBorderSpacing,
    kCSSPropertyBorderStyle,
    kCSSPropertyBorderTop,
    kCSSPropertyBorderTopColor,
    kCSSPropertyBorderTopLeftRadius,
    kCSSPropertyBorderTopLeftRadiusX,
    kCSSPropertyBorderTopLeftRadiusY,
    kCSSPropertyBorderTopRightRadius,
    kCSSPropertyBorderTopRightRadiusX,
    kCSSPropertyBorderTopRightRadiusY,
    kCSSPropertyBorderTopStyle,
    kCSSPropertyBorderTopWidth,
    kCSSPropertyBorderWidth,
    kCSSPropertyBottom,
    kCSSPropertyBoxSizing,
    kCSSPropertyClear,
    kCSSPropertyColor,
    kCSSPropertyContent,
    kCSSPropertyCursor,
    kCSSPropertyDisplay,
    kCSSPropertyFloat,
    kCSSPropertyFont,
    kCSSPropertyFontFamily,
    kCSSPropertyFontSize,
    kCSSPropertyFontStyle,
    kCSSPropertyFontVariant,
    kCSSPropertyFontWeight,
    kCSSPropertyHeight,
    kCSSPropertyLeft,
    kCSSPropertyLineHeight,
    kCSSPropertyListStyle,
    kCSSPropertyListStyleImage,
    kCSSPropertyListStyleImageBaseurl,
    kCSSPropertyListStylePosition,
    kCSSPropertyListStyleType,
    kCSSPropertyMargin,
    kCSSPropertyMarginBottom,
    kCSSPropertyMarginLeft,
    kCSSPropertyMarginRight,
    kCSSPropertyMarginTop,
    kCSSPropertyMaxHeight,
    kCSSPropertyMaxWidth,
    kCSSPropertyMinHeight,
    kCSSPropertyMinWidth,
    kCSSPropertyOverflow,
    kCSSPropertyPadding,
    kCSSPropertyPaddingBottom,
    kCSSPropertyPaddingLeft,
    kCSSPropertyPaddingRight,
    kCSSPropertyPaddingTop,
    kCSSPropertyPosition,
    kCSSPropertyRight,
    kCSSPropertyTextAlign,
    kCSSPropertyTextDecoration,
    kCSSPropertyTextIndent,
    kCSSPropertyTextShadow,
    kCSSPropertyTextTransform,
    kCSSPropertyTop,
    kCSSPropertyVerticalAlign,
    kCSSPropertyVisibility,
    kCSSPropertyWhiteSpace,
    kCSSPropertyWidth,
    kCSSPropertyZIndex,
    kCSSPropertyUnknown
};

using CSSKeyword = int;

constexpr int kCSSKeywordNone = -1;

using KeywordVector = std::vector<std::pair<String, CSSKeyword>>;

String css_property_string(CSSProperty property);

CSSProperty css_property_from_string(const String& name);

// Returns the default (or initial) value of the specified CSS property as a
// string.  May return nullptr if the default value is unspecified.
const tchar_t* css_property_default(CSSProperty property);

// Returns the default (or initial) value of the specified CSS property as a
// CSSValue.  May return nullptr if the default value is unspecified.
const CSSValue* css_property_default_value(CSSProperty property);

// Returns true if the specified CSS property is an inherited property, false
// otherwise.  When no value for an inherited property is specified, elements
// get the computed value of that property from its parent element.
bool css_property_inherited(CSSProperty property);

// Returns the value type of the CSS property (e.g., kCSSPropertyColor has
// value type kCSSValueColor).
CSSValueType css_property_value_type(CSSProperty property);

const KeywordVector& css_property_keywords(CSSProperty property);

} // namespace litehtml

#endif // LITEHTML_CSS_CSS_PROPERTY_H__
