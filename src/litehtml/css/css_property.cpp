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

#include "litehtml/css/css_property.h"

#include "litehtml/background.h"
#include "litehtml/css/css_value.h"

namespace litehtml {

String css_property_string(CSSProperty property)
{
    switch (property) {
        case kCSSPropertyNone:
            return "none";
        case kCSSPropertyLitehtmlBorderSpacingX:
            return "-litehtml-border-spacing-x";
        case kCSSPropertyLitehtmlBorderSpacingY:
            return "-litehtml-border-spacing-y";
        case kCSSPropertyBackground:
            return "background";
        case kCSSPropertyBackgroundAttachment:
            return "background-attachment";
        case kCSSPropertyBackgroundClip:
            return "background-clip";
        case kCSSPropertyBackgroundColor:
            return "background-color";
        case kCSSPropertyBackgroundImage:
            return "background-image";
        case kCSSPropertyBackgroundImageBaseurl:
            return "background-image-baseurl";
        case kCSSPropertyBackgroundOrigin:
            return "background-origin";
        case kCSSPropertyBackgroundPosition:
            return "background-position";
        case kCSSPropertyBackgroundRepeat:
            return "background-repeat";
        case kCSSPropertyBackgroundSize:
            return "background-size";
        case kCSSPropertyBorder:
            return "border";
        case kCSSPropertyBorderBottom:
            return "border-bottom";
        case kCSSPropertyBorderBottomColor:
            return "border-bottom-color";
        case kCSSPropertyBorderBottomLeftRadius:
            return "border-bottom-left-radius";
        case kCSSPropertyBorderBottomLeftRadiusX:
            return "border-bottom-left-radius-x";
        case kCSSPropertyBorderBottomLeftRadiusY:
            return "border-bottom-left-radius-y";
        case kCSSPropertyBorderBottomRightRadius:
            return "border-bottom-right-radius";
        case kCSSPropertyBorderBottomRightRadiusX:
            return "border-bottom-right-radius-x";
        case kCSSPropertyBorderBottomRightRadiusY:
            return "border-bottom-right-radius-y";
        case kCSSPropertyBorderBottomStyle:
            return "border-bottom-style";
        case kCSSPropertyBorderBottomWidth:
            return "border-bottom-width";
        case kCSSPropertyBorderCollapse:
            return "border-collapse";
        case kCSSPropertyBorderColor:
            return "border-color";
        case kCSSPropertyBorderLeft:
            return "border-left";
        case kCSSPropertyBorderLeftColor:
            return "border-left-color";
        case kCSSPropertyBorderLeftStyle:
            return "border-left-style";
        case kCSSPropertyBorderLeftWidth:
            return "border-left-width";
        case kCSSPropertyBorderRadius:
            return "border-radius";
        case kCSSPropertyBorderRadiusX:
            return "border-radius-x";
        case kCSSPropertyBorderRadiusY:
            return "border-radius-y";
        case kCSSPropertyBorderRight:
            return "border-right";
        case kCSSPropertyBorderRightColor:
            return "border-right-color";
        case kCSSPropertyBorderRightStyle:
            return "border-right-style";
        case kCSSPropertyBorderRightWidth:
            return "border-right-width";
        case kCSSPropertyBorderSpacing:
            return "border-spacing";
        case kCSSPropertyBorderStyle:
            return "border-style";
        case kCSSPropertyBorderTop:
            return "border-top";
        case kCSSPropertyBorderTopColor:
            return "border-top-color";
        case kCSSPropertyBorderTopLeftRadius:
            return "border-top-left-radius";
        case kCSSPropertyBorderTopLeftRadiusX:
            return "border-top-left-radius-x";
        case kCSSPropertyBorderTopLeftRadiusY:
            return "border-top-left-radius-y";
        case kCSSPropertyBorderTopRightRadius:
            return "border-top-right-radius";
        case kCSSPropertyBorderTopRightRadiusX:
            return "border-top-right-radius-x";
        case kCSSPropertyBorderTopRightRadiusY:
            return "border-top-right-radius-y";
        case kCSSPropertyBorderTopStyle:
            return "border-top-style";
        case kCSSPropertyBorderTopWidth:
            return "border-top-width";
        case kCSSPropertyBorderWidth:
            return "border-width";
        case kCSSPropertyBottom:
            return "bottom";
        case kCSSPropertyBoxSizing:
            return "box-sizing";
        case kCSSPropertyClear:
            return "clear";
        case kCSSPropertyColor:
            return "color";
        case kCSSPropertyContent:
            return "content";
        case kCSSPropertyCursor:
            return "cursor";
        case kCSSPropertyDisplay:
            return "display";
        case kCSSPropertyFloat:
            return "float";
        case kCSSPropertyFont:
            return "font";
        case kCSSPropertyFontFamily:
            return "font-family";
        case kCSSPropertyFontSize:
            return "font-size";
        case kCSSPropertyFontStyle:
            return "font-style";
        case kCSSPropertyFontVariant:
            return "font-variant";
        case kCSSPropertyFontWeight:
            return "font-weight";
        case kCSSPropertyHeight:
            return "height";
        case kCSSPropertyLeft:
            return "left";
        case kCSSPropertyLineHeight:
            return "line-height";
        case kCSSPropertyListStyle:
            return "list-style";
        case kCSSPropertyListStyleImage:
            return "list-style-image";
        case kCSSPropertyListStyleImageBaseurl:
            return "list-style-image-baseurl";
        case kCSSPropertyListStylePosition:
            return "list-style-position";
        case kCSSPropertyListStyleType:
            return "list-style-type";
        case kCSSPropertyMargin:
            return "margin";
        case kCSSPropertyMarginBottom:
            return "margin-bottom";
        case kCSSPropertyMarginLeft:
            return "margin-left";
        case kCSSPropertyMarginRight:
            return "margin-right";
        case kCSSPropertyMarginTop:
            return "margin-top";
        case kCSSPropertyMaxHeight:
            return "max-height";
        case kCSSPropertyMaxWidth:
            return "max-width";
        case kCSSPropertyMinHeight:
            return "min-height";
        case kCSSPropertyMinWidth:
            return "min-width";
        case kCSSPropertyOverflow:
            return "overflow";
        case kCSSPropertyPadding:
            return "padding";
        case kCSSPropertyPaddingBottom:
            return "padding-bottom";
        case kCSSPropertyPaddingLeft:
            return "padding-left";
        case kCSSPropertyPaddingRight:
            return "padding-right";
        case kCSSPropertyPaddingTop:
            return "padding-top";
        case kCSSPropertyPosition:
            return "position";
        case kCSSPropertyRight:
            return "right";
        case kCSSPropertyTextAlign:
            return "text-align";
        case kCSSPropertyTextDecoration:
            return "text-decoration";
        case kCSSPropertyTextIndent:
            return "text-indent";
        case kCSSPropertyTextShadow:
            return "text-shadow";
        case kCSSPropertyTextTransform:
            return "text-transform";
        case kCSSPropertyTop:
            return "top";
        case kCSSPropertyVerticalAlign:
            return "vertical-align";
        case kCSSPropertyVisibility:
            return "visibility";
        case kCSSPropertyWhiteSpace:
            return "white-space";
        case kCSSPropertyWidth:
            return "width";
        case kCSSPropertyZIndex:
            return "z-index";
        case kCSSPropertyUnknown:
        default:
            return "unknown";
    }
}

CSSProperty css_property_from_string(const String& name)
{
    if (name == "-litehtml-border-spacing-x") {
        return kCSSPropertyLitehtmlBorderSpacingX;
    }
    if (name == "-litehtml-border-spacing-y") {
        return kCSSPropertyLitehtmlBorderSpacingY;
    }
    if (name == "background") {
        return kCSSPropertyBackground;
    }
    if (name == "background-attachment") {
        return kCSSPropertyBackgroundAttachment;
    }
    if (name == "background-clip") {
        return kCSSPropertyBackgroundClip;
    }
    if (name == "background-color") {
        return kCSSPropertyBackgroundColor;
    }
    if (name == "background-image") {
        return kCSSPropertyBackgroundImage;
    }
    if (name == "background-image-baseurl") {
        return kCSSPropertyBackgroundImageBaseurl;
    }
    if (name == "background-origin") {
        return kCSSPropertyBackgroundOrigin;
    }
    if (name == "background-position") {
        return kCSSPropertyBackgroundPosition;
    }
    if (name == "background-repeat") {
        return kCSSPropertyBackgroundRepeat;
    }
    if (name == "background-size") {
        return kCSSPropertyBackgroundSize;
    }
    if (name == "border") {
        return kCSSPropertyBorder;
    }
    if (name == "border-bottom") {
        return kCSSPropertyBorderBottom;
    }
    if (name == "border-bottom-color") {
        return kCSSPropertyBorderBottomColor;
    }
    if (name == "border-bottom-left-radius") {
        return kCSSPropertyBorderBottomLeftRadius;
    }
    if (name == "border-bottom-left-radius-x") {
        return kCSSPropertyBorderBottomLeftRadiusX;
    }
    if (name == "border-bottom-left-radius-y") {
        return kCSSPropertyBorderBottomLeftRadiusY;
    }
    if (name == "border-bottom-right-radius") {
        return kCSSPropertyBorderBottomRightRadius;
    }
    if (name == "border-bottom-right-radius-x") {
        return kCSSPropertyBorderBottomRightRadiusX;
    }
    if (name == "border-bottom-right-radius-y") {
        return kCSSPropertyBorderBottomRightRadiusY;
    }
    if (name == "border-bottom-style") {
        return kCSSPropertyBorderBottomStyle;
    }
    if (name == "border-bottom-width") {
        return kCSSPropertyBorderBottomWidth;
    }
    if (name == "border-collapse") {
        return kCSSPropertyBorderCollapse;
    }
    if (name == "border-color") {
        return kCSSPropertyBorderColor;
    }
    if (name == "border-left") {
        return kCSSPropertyBorderLeft;
    }
    if (name == "border-left-color") {
        return kCSSPropertyBorderLeftColor;
    }
    if (name == "border-left-style") {
        return kCSSPropertyBorderLeftStyle;
    }
    if (name == "border-left-width") {
        return kCSSPropertyBorderLeftWidth;
    }
    if (name == "border-radius") {
        return kCSSPropertyBorderRadius;
    }
    if (name == "border-radius-x") {
        return kCSSPropertyBorderRadiusX;
    }
    if (name == "border-radius-y") {
        return kCSSPropertyBorderRadiusY;
    }
    if (name == "border-right") {
        return kCSSPropertyBorderRight;
    }
    if (name == "border-right-color") {
        return kCSSPropertyBorderRightColor;
    }
    if (name == "border-right-style") {
        return kCSSPropertyBorderRightStyle;
    }
    if (name == "border-right-width") {
        return kCSSPropertyBorderRightWidth;
    }
    if (name == "border-spacing") {
        return kCSSPropertyBorderSpacing;
    }
    if (name == "border-style") {
        return kCSSPropertyBorderStyle;
    }
    if (name == "border-top") {
        return kCSSPropertyBorderTop;
    }
    if (name == "border-top-color") {
        return kCSSPropertyBorderTopColor;
    }
    if (name == "border-top-left-radius") {
        return kCSSPropertyBorderTopLeftRadius;
    }
    if (name == "border-top-left-radius-x") {
        return kCSSPropertyBorderTopLeftRadiusX;
    }
    if (name == "border-top-left-radius-y") {
        return kCSSPropertyBorderTopLeftRadiusY;
    }
    if (name == "border-top-right-radius") {
        return kCSSPropertyBorderTopRightRadius;
    }
    if (name == "border-top-right-radius-x") {
        return kCSSPropertyBorderTopRightRadiusX;
    }
    if (name == "border-top-right-radius-y") {
        return kCSSPropertyBorderTopRightRadiusY;
    }
    if (name == "border-top-style") {
        return kCSSPropertyBorderTopStyle;
    }
    if (name == "border-top-width") {
        return kCSSPropertyBorderTopWidth;
    }
    if (name == "border-width") {
        return kCSSPropertyBorderWidth;
    }
    if (name == "bottom") {
        return kCSSPropertyBottom;
    }
    if (name == "box-sizing") {
        return kCSSPropertyBoxSizing;
    }
    if (name == "clear") {
        return kCSSPropertyClear;
    }
    if (name == "color") {
        return kCSSPropertyColor;
    }
    if (name == "content") {
        return kCSSPropertyContent;
    }
    if (name == "cursor") {
        return kCSSPropertyCursor;
    }
    if (name == "display") {
        return kCSSPropertyDisplay;
    }
    if (name == "float") {
        return kCSSPropertyFloat;
    }
    if (name == "font") {
        return kCSSPropertyFont;
    }
    if (name == "font-family") {
        return kCSSPropertyFontFamily;
    }
    if (name == "font-size") {
        return kCSSPropertyFontSize;
    }
    if (name == "font-style") {
        return kCSSPropertyFontStyle;
    }
    if (name == "font-variant") {
        return kCSSPropertyFontVariant;
    }
    if (name == "font-weight") {
        return kCSSPropertyFontWeight;
    }
    if (name == "height") {
        return kCSSPropertyHeight;
    }
    if (name == "left") {
        return kCSSPropertyLeft;
    }
    if (name == "line-height") {
        return kCSSPropertyLineHeight;
    }
    if (name == "list-style") {
        return kCSSPropertyListStyle;
    }
    if (name == "list-style-image") {
        return kCSSPropertyListStyleImage;
    }
    if (name == "list-style-image-baseurl") {
        return kCSSPropertyListStyleImageBaseurl;
    }
    if (name == "list-style-position") {
        return kCSSPropertyListStylePosition;
    }
    if (name == "list-style-type") {
        return kCSSPropertyListStyleType;
    }
    if (name == "margin") {
        return kCSSPropertyMargin;
    }
    if (name == "margin-bottom") {
        return kCSSPropertyMarginBottom;
    }
    if (name == "margin-left") {
        return kCSSPropertyMarginLeft;
    }
    if (name == "margin-right") {
        return kCSSPropertyMarginRight;
    }
    if (name == "margin-top") {
        return kCSSPropertyMarginTop;
    }
    if (name == "max-height") {
        return kCSSPropertyMaxHeight;
    }
    if (name == "max-width") {
        return kCSSPropertyMaxWidth;
    }
    if (name == "min-height") {
        return kCSSPropertyMinHeight;
    }
    if (name == "min-width") {
        return kCSSPropertyMinWidth;
    }
    if (name == "overflow") {
        return kCSSPropertyOverflow;
    }
    if (name == "padding") {
        return kCSSPropertyPadding;
    }
    if (name == "padding-bottom") {
        return kCSSPropertyPaddingBottom;
    }
    if (name == "padding-left") {
        return kCSSPropertyPaddingLeft;
    }
    if (name == "padding-right") {
        return kCSSPropertyPaddingRight;
    }
    if (name == "padding-top") {
        return kCSSPropertyPaddingTop;
    }
    if (name == "position") {
        return kCSSPropertyPosition;
    }
    if (name == "right") {
        return kCSSPropertyRight;
    }
    if (name == "text-align") {
        return kCSSPropertyTextAlign;
    }
    if (name == "text-decoration") {
        return kCSSPropertyTextDecoration;
    }
    if (name == "text-indent") {
        return kCSSPropertyTextIndent;
    }
    if (name == "text-shadow") {
        return kCSSPropertyTextShadow;
    }
    if (name == "text-transform") {
        return kCSSPropertyTextTransform;
    }
    if (name == "top") {
        return kCSSPropertyTop;
    }
    if (name == "vertical-align") {
        return kCSSPropertyVerticalAlign;
    }
    if (name == "visibility") {
        return kCSSPropertyVisibility;
    }
    if (name == "white-space") {
        return kCSSPropertyWhiteSpace;
    }
    if (name == "width") {
        return kCSSPropertyWidth;
    }
    if (name == "z-index") {
        return kCSSPropertyZIndex;
    }

    return kCSSPropertyUnknown;
}

const char* css_property_default(CSSProperty property)
{
    switch (property) {
        case kCSSPropertyNone:
            return nullptr;
        case kCSSPropertyLitehtmlBorderSpacingX:
            return "0px";
        case kCSSPropertyLitehtmlBorderSpacingY:
            return "0px";
        case kCSSPropertyBackground:
            return nullptr;
        case kCSSPropertyBackgroundAttachment:
            return "scroll";
        case kCSSPropertyBackgroundClip:
            return "border-box";
        case kCSSPropertyBackgroundColor:
            return "transparent";
        case kCSSPropertyBackgroundImage:
            return "";
        case kCSSPropertyBackgroundImageBaseurl:
            return "";
        case kCSSPropertyBackgroundOrigin:
            return "padding-box";
        case kCSSPropertyBackgroundPosition:
            return "0% 0%";
        case kCSSPropertyBackgroundRepeat:
            return "repeat";
        case kCSSPropertyBackgroundSize:
            return "auto";
        case kCSSPropertyBorder:
            return nullptr;
        case kCSSPropertyBorderBottom:
            return nullptr;
        case kCSSPropertyBorderBottomColor:
            return "currentcolor";
        case kCSSPropertyBorderBottomLeftRadius:
            return "0";
        case kCSSPropertyBorderBottomLeftRadiusX:
            return "0";
        case kCSSPropertyBorderBottomLeftRadiusY:
            return "0";
        case kCSSPropertyBorderBottomRightRadius:
            return "0";
        case kCSSPropertyBorderBottomRightRadiusX:
            return "0";
        case kCSSPropertyBorderBottomRightRadiusY:
            return "0";
        case kCSSPropertyBorderBottomStyle:
            return "none";
        case kCSSPropertyBorderBottomWidth:
            return "medium";
        case kCSSPropertyBorderCollapse:
            return "separate";
        case kCSSPropertyBorderColor:
            return nullptr;
        case kCSSPropertyBorderLeft:
            return nullptr;
        case kCSSPropertyBorderLeftColor:
            return "currentcolor";
        case kCSSPropertyBorderLeftStyle:
            return "none";
        case kCSSPropertyBorderLeftWidth:
            return "medium";
        case kCSSPropertyBorderRadius:
            return nullptr;
        case kCSSPropertyBorderRadiusX:
            return "0";
        case kCSSPropertyBorderRadiusY:
            return "0";
        case kCSSPropertyBorderRight:
            return nullptr;
        case kCSSPropertyBorderRightColor:
            return "currentcolor";
        case kCSSPropertyBorderRightStyle:
            return "none";
        case kCSSPropertyBorderRightWidth:
            return "medium";
        case kCSSPropertyBorderSpacing:
            return "0";
        case kCSSPropertyBorderStyle:
            return nullptr;
        case kCSSPropertyBorderTop:
            return nullptr;
        case kCSSPropertyBorderTopColor:
            return "currentcolor";
        case kCSSPropertyBorderTopLeftRadius:
            return "0";
        case kCSSPropertyBorderTopLeftRadiusX:
            return "0";
        case kCSSPropertyBorderTopLeftRadiusY:
            return "0";
        case kCSSPropertyBorderTopRightRadius:
            return "0";
        case kCSSPropertyBorderTopRightRadiusX:
            return "0";
        case kCSSPropertyBorderTopRightRadiusY:
            return "0";
        case kCSSPropertyBorderTopStyle:
            return "none";
        case kCSSPropertyBorderTopWidth:
            return "medium";
        case kCSSPropertyBorderWidth:
            return nullptr;
        case kCSSPropertyBottom:
            return "auto";
        case kCSSPropertyBoxSizing:
            return "content-box";
        case kCSSPropertyClear:
            return "none";
        case kCSSPropertyColor:
            return "black";
        case kCSSPropertyContent:
            return "";
        case kCSSPropertyCursor:
            return "auto";
        case kCSSPropertyDisplay:
            return "inline";
        case kCSSPropertyFloat:
            return "none";
        case kCSSPropertyFont:
            return nullptr;
        case kCSSPropertyFontFamily:
            return "inherit";
        case kCSSPropertyFontSize:
            return "medium";
        case kCSSPropertyFontStyle:
            return "normal";
        case kCSSPropertyFontVariant:
            return "normal";
        case kCSSPropertyFontWeight:
            return "normal";
        case kCSSPropertyHeight:
            return "auto";
        case kCSSPropertyLeft:
            return "auto";
        case kCSSPropertyLineHeight:
            return "normal";
        case kCSSPropertyListStyle:
            return nullptr;
        case kCSSPropertyListStyleImage:
            return "none";
        case kCSSPropertyListStyleImageBaseurl:
            return "";
        case kCSSPropertyListStylePosition:
            return "outside";
        case kCSSPropertyListStyleType:
            return "disc";
        case kCSSPropertyMargin:
            return nullptr;
        case kCSSPropertyMarginBottom:
            return "0";
        case kCSSPropertyMarginLeft:
            return "0";
        case kCSSPropertyMarginRight:
            return "0";
        case kCSSPropertyMarginTop:
            return "0";
        case kCSSPropertyMaxHeight:
            return "none";
        case kCSSPropertyMaxWidth:
            return "none";
        case kCSSPropertyMinHeight:
            return "0";
        case kCSSPropertyMinWidth:
            return "0";
        case kCSSPropertyOverflow:
            return "visible";
        case kCSSPropertyPadding:
            return nullptr;
        case kCSSPropertyPaddingBottom:
            return "0";
        case kCSSPropertyPaddingLeft:
            return "0";
        case kCSSPropertyPaddingRight:
            return "0";
        case kCSSPropertyPaddingTop:
            return "0";
        case kCSSPropertyPosition:
            return "static";
        case kCSSPropertyRight:
            return "auto";
        case kCSSPropertyTextAlign:
            return "left";
        case kCSSPropertyTextDecoration:
            return "none";
        case kCSSPropertyTextIndent:
            return "0";
        case kCSSPropertyTextShadow:
            return "none";
        case kCSSPropertyTextTransform:
            return "none";
        case kCSSPropertyTop:
            return "auto";
        case kCSSPropertyVerticalAlign:
            return "baseline";
        case kCSSPropertyVisibility:
            return "visible";
        case kCSSPropertyWhiteSpace:
            return "normal";
        case kCSSPropertyWidth:
            return "auto";
        case kCSSPropertyZIndex:
            return "auto";
        case kCSSPropertyUnknown:
        default:
            return nullptr;
    }
}

const CSSValue* css_property_default_value(CSSProperty property)
{
    switch (property) {
        case kCSSPropertyNone:
            return nullptr;
        case kCSSPropertyLitehtmlBorderSpacingX: {
            static const CSSValue* default_value = CSSValue::factory(property, "0px", false);
            return default_value;
        }
        case kCSSPropertyLitehtmlBorderSpacingY: {
            static const CSSValue* default_value = CSSValue::factory(property, "0px", false);
            return default_value;
        }
        case kCSSPropertyBackground: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyBackgroundAttachment: {
            static const CSSValue* default_value = CSSValue::factory(property, "scroll", false);
            return default_value;
        }
        case kCSSPropertyBackgroundClip: {
            static const CSSValue* default_value = CSSValue::factory(property, "border-box", false);
            return default_value;
        }
        case kCSSPropertyBackgroundColor: {
            static const CSSValue* default_value = CSSValue::factory(property, "transparent", false);
            return default_value;
        }
        case kCSSPropertyBackgroundImage: {
            static const CSSValue* default_value = CSSValue::factory(property, "", false);
            return default_value;
        }
        case kCSSPropertyBackgroundImageBaseurl: {
            static const CSSValue* default_value = CSSValue::factory(property, "", false);
            return default_value;
        }
        case kCSSPropertyBackgroundOrigin: {
            static const CSSValue* default_value = CSSValue::factory(property, "padding-box", false);
            return default_value;
        }
        case kCSSPropertyBackgroundPosition: {
            static const CSSValue* default_value = CSSValue::factory(property, "0% 0%", false);
            return default_value;
        }
        case kCSSPropertyBackgroundRepeat: {
            static const CSSValue* default_value = CSSValue::factory(property, "repeat", false);
            return default_value;
        }
        case kCSSPropertyBackgroundSize: {
            static const CSSValue* default_value = CSSValue::factory(property, "auto", false);
            return default_value;
        }
        case kCSSPropertyBorder: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyBorderBottom: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyBorderBottomColor: {
            static const CSSValue* default_value = CSSValue::factory(property, "currentcolor", false);
            return default_value;
        }
        case kCSSPropertyBorderBottomLeftRadius: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderBottomLeftRadiusX: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderBottomLeftRadiusY: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderBottomRightRadius: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderBottomRightRadiusX: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderBottomRightRadiusY: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderBottomStyle: {
            static const CSSValue* default_value = CSSValue::factory(property, "none", false);
            return default_value;
        }
        case kCSSPropertyBorderBottomWidth: {
            static const CSSValue* default_value = CSSValue::factory(property, "medium", false);
            return default_value;
        }
        case kCSSPropertyBorderCollapse: {
            static const CSSValue* default_value = CSSValue::factory(property, "separate", false);
            return default_value;
        }
        case kCSSPropertyBorderColor: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyBorderLeft: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyBorderLeftColor: {
            static const CSSValue* default_value = CSSValue::factory(property, "currentcolor", false);
            return default_value;
        }
        case kCSSPropertyBorderLeftStyle: {
            static const CSSValue* default_value = CSSValue::factory(property, "none", false);
            return default_value;
        }
        case kCSSPropertyBorderLeftWidth: {
            static const CSSValue* default_value = CSSValue::factory(property, "medium", false);
            return default_value;
        }
        case kCSSPropertyBorderRadius: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyBorderRadiusX: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderRadiusY: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderRight: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyBorderRightColor: {
            static const CSSValue* default_value = CSSValue::factory(property, "currentcolor", false);
            return default_value;
        }
        case kCSSPropertyBorderRightStyle: {
            static const CSSValue* default_value = CSSValue::factory(property, "none", false);
            return default_value;
        }
        case kCSSPropertyBorderRightWidth: {
            static const CSSValue* default_value = CSSValue::factory(property, "medium", false);
            return default_value;
        }
        case kCSSPropertyBorderSpacing: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderStyle: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyBorderTop: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyBorderTopColor: {
            static const CSSValue* default_value = CSSValue::factory(property, "currentcolor", false);
            return default_value;
        }
        case kCSSPropertyBorderTopLeftRadius: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderTopLeftRadiusX: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderTopLeftRadiusY: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderTopRightRadius: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderTopRightRadiusX: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderTopRightRadiusY: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyBorderTopStyle: {
            static const CSSValue* default_value = CSSValue::factory(property, "none", false);
            return default_value;
        }
        case kCSSPropertyBorderTopWidth: {
            static const CSSValue* default_value = CSSValue::factory(property, "medium", false);
            return default_value;
        }
        case kCSSPropertyBorderWidth: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyBottom: {
            static const CSSValue* default_value = CSSValue::factory(property, "auto", false);
            return default_value;
        }
        case kCSSPropertyBoxSizing: {
            static const CSSValue* default_value = CSSValue::factory(property, "content-box", false);
            return default_value;
        }
        case kCSSPropertyClear: {
            static const CSSValue* default_value = CSSValue::factory(property, "none", false);
            return default_value;
        }
        case kCSSPropertyColor: {
            static const CSSValue* default_value = CSSValue::factory(property, "black", false);
            return default_value;
        }
        case kCSSPropertyContent: {
            static const CSSValue* default_value = CSSValue::factory(property, "", false);
            return default_value;
        }
        case kCSSPropertyCursor: {
            static const CSSValue* default_value = CSSValue::factory(property, "auto", false);
            return default_value;
        }
        case kCSSPropertyDisplay: {
            static const CSSValue* default_value = CSSValue::factory(property, "inline", false);
            return default_value;
        }
        case kCSSPropertyFloat: {
            static const CSSValue* default_value = CSSValue::factory(property, "none", false);
            return default_value;
        }
        case kCSSPropertyFont: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyFontFamily: {
            static const CSSValue* default_value = CSSValue::factory(property, "inherit", false);
            return default_value;
        }
        case kCSSPropertyFontSize: {
            static const CSSValue* default_value = CSSValue::factory(property, "medium", false);
            return default_value;
        }
        case kCSSPropertyFontStyle: {
            static const CSSValue* default_value = CSSValue::factory(property, "normal", false);
            return default_value;
        }
        case kCSSPropertyFontVariant: {
            static const CSSValue* default_value = CSSValue::factory(property, "normal", false);
            return default_value;
        }
        case kCSSPropertyFontWeight: {
            static const CSSValue* default_value = CSSValue::factory(property, "normal", false);
            return default_value;
        }
        case kCSSPropertyHeight: {
            static const CSSValue* default_value = CSSValue::factory(property, "auto", false);
            return default_value;
        }
        case kCSSPropertyLeft: {
            static const CSSValue* default_value = CSSValue::factory(property, "auto", false);
            return default_value;
        }
        case kCSSPropertyLineHeight: {
            static const CSSValue* default_value = CSSValue::factory(property, "normal", false);
            return default_value;
        }
        case kCSSPropertyListStyle: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyListStyleImage: {
            static const CSSValue* default_value = CSSValue::factory(property, "none", false);
            return default_value;
        }
        case kCSSPropertyListStyleImageBaseurl: {
            static const CSSValue* default_value = CSSValue::factory(property, "", false);
            return default_value;
        }
        case kCSSPropertyListStylePosition: {
            static const CSSValue* default_value = CSSValue::factory(property, "outside", false);
            return default_value;
        }
        case kCSSPropertyListStyleType: {
            static const CSSValue* default_value = CSSValue::factory(property, "disc", false);
            return default_value;
        }
        case kCSSPropertyMargin: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyMarginBottom: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyMarginLeft: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyMarginRight: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyMarginTop: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyMaxHeight: {
            static const CSSValue* default_value = CSSValue::factory(property, "none", false);
            return default_value;
        }
        case kCSSPropertyMaxWidth: {
            static const CSSValue* default_value = CSSValue::factory(property, "none", false);
            return default_value;
        }
        case kCSSPropertyMinHeight: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyMinWidth: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyOverflow: {
            static const CSSValue* default_value = CSSValue::factory(property, "visible", false);
            return default_value;
        }
        case kCSSPropertyPadding: {
            static const CSSValue* default_value = CSSValue::factory(property, nullptr, false);
            return default_value;
        }
        case kCSSPropertyPaddingBottom: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyPaddingLeft: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyPaddingRight: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyPaddingTop: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyPosition: {
            static const CSSValue* default_value = CSSValue::factory(property, "static", false);
            return default_value;
        }
        case kCSSPropertyRight: {
            static const CSSValue* default_value = CSSValue::factory(property, "auto", false);
            return default_value;
        }
        case kCSSPropertyTextAlign: {
            static const CSSValue* default_value = CSSValue::factory(property, "left", false);
            return default_value;
        }
        case kCSSPropertyTextDecoration: {
            static const CSSValue* default_value = CSSValue::factory(property, "none", false);
            return default_value;
        }
        case kCSSPropertyTextIndent: {
            static const CSSValue* default_value = CSSValue::factory(property, "0", false);
            return default_value;
        }
        case kCSSPropertyTextShadow: {
            static const CSSValue* default_value = CSSValue::factory(property, "none", false);
            return default_value;
        }
        case kCSSPropertyTextTransform: {
            static const CSSValue* default_value = CSSValue::factory(property, "none", false);
            return default_value;
        }
        case kCSSPropertyTop: {
            static const CSSValue* default_value = CSSValue::factory(property, "auto", false);
            return default_value;
        }
        case kCSSPropertyVerticalAlign: {
            static const CSSValue* default_value = CSSValue::factory(property, "baseline", false);
            return default_value;
        }
        case kCSSPropertyVisibility: {
            static const CSSValue* default_value = CSSValue::factory(property, "visible", false);
            return default_value;
        }
        case kCSSPropertyWhiteSpace: {
            static const CSSValue* default_value = CSSValue::factory(property, "normal", false);
            return default_value;
        }
        case kCSSPropertyWidth: {
            static const CSSValue* default_value = CSSValue::factory(property, "auto", false);
            return default_value;
        }
        case kCSSPropertyZIndex: {
            static const CSSValue* default_value = CSSValue::factory(property, "auto", false);
            return default_value;
        }
        case kCSSPropertyUnknown:
        default:
            return nullptr;
    }
}


bool css_property_inherited(CSSProperty property)
{
    switch (property) {
        case kCSSPropertyNone:
            return false;
        case kCSSPropertyLitehtmlBorderSpacingX:
            return true;
        case kCSSPropertyLitehtmlBorderSpacingY:
            return true;
        case kCSSPropertyBackground:
            return true;
        case kCSSPropertyBackgroundAttachment:
            return false;
        case kCSSPropertyBackgroundClip:
            return false;
        case kCSSPropertyBackgroundColor:
            return false;
        case kCSSPropertyBackgroundImage:
            return false;
        case kCSSPropertyBackgroundImageBaseurl:
            return false;
        case kCSSPropertyBackgroundOrigin:
            return false;
        case kCSSPropertyBackgroundPosition:
            return false;
        case kCSSPropertyBackgroundRepeat:
            return false;
        case kCSSPropertyBackgroundSize:
            return false;
        case kCSSPropertyBorder:
            return true;
        case kCSSPropertyBorderBottom:
            return true;
        case kCSSPropertyBorderBottomColor:
            return false;
        case kCSSPropertyBorderBottomLeftRadius:
            return false;
        case kCSSPropertyBorderBottomLeftRadiusX:
            return false;
        case kCSSPropertyBorderBottomLeftRadiusY:
            return false;
        case kCSSPropertyBorderBottomRightRadius:
            return false;
        case kCSSPropertyBorderBottomRightRadiusX:
            return false;
        case kCSSPropertyBorderBottomRightRadiusY:
            return false;
        case kCSSPropertyBorderBottomStyle:
            return false;
        case kCSSPropertyBorderBottomWidth:
            return false;
        case kCSSPropertyBorderCollapse:
            return true;
        case kCSSPropertyBorderColor:
            return true;
        case kCSSPropertyBorderLeft:
            return true;
        case kCSSPropertyBorderLeftColor:
            return false;
        case kCSSPropertyBorderLeftStyle:
            return false;
        case kCSSPropertyBorderLeftWidth:
            return false;
        case kCSSPropertyBorderRadius:
            return true;
        case kCSSPropertyBorderRadiusX:
            return false;
        case kCSSPropertyBorderRadiusY:
            return false;
        case kCSSPropertyBorderRight:
            return true;
        case kCSSPropertyBorderRightColor:
            return false;
        case kCSSPropertyBorderRightStyle:
            return false;
        case kCSSPropertyBorderRightWidth:
            return false;
        case kCSSPropertyBorderSpacing:
            return true;
        case kCSSPropertyBorderStyle:
            return true;
        case kCSSPropertyBorderTop:
            return true;
        case kCSSPropertyBorderTopColor:
            return false;
        case kCSSPropertyBorderTopLeftRadius:
            return false;
        case kCSSPropertyBorderTopLeftRadiusX:
            return false;
        case kCSSPropertyBorderTopLeftRadiusY:
            return false;
        case kCSSPropertyBorderTopRightRadius:
            return false;
        case kCSSPropertyBorderTopRightRadiusX:
            return false;
        case kCSSPropertyBorderTopRightRadiusY:
            return false;
        case kCSSPropertyBorderTopStyle:
            return false;
        case kCSSPropertyBorderTopWidth:
            return false;
        case kCSSPropertyBorderWidth:
            return true;
        case kCSSPropertyBottom:
            return false;
        case kCSSPropertyBoxSizing:
            return false;
        case kCSSPropertyClear:
            return false;
        case kCSSPropertyColor:
            return true;
        case kCSSPropertyContent:
            return false;
        case kCSSPropertyCursor:
            return true;
        case kCSSPropertyDisplay:
            return false;
        case kCSSPropertyFloat:
            return false;
        case kCSSPropertyFont:
            return true;
        case kCSSPropertyFontFamily:
            return true;
        case kCSSPropertyFontSize:
            return true;
        case kCSSPropertyFontStyle:
            return true;
        case kCSSPropertyFontVariant:
            return true;
        case kCSSPropertyFontWeight:
            return true;
        case kCSSPropertyHeight:
            return false;
        case kCSSPropertyLeft:
            return false;
        case kCSSPropertyLineHeight:
            return true;
        case kCSSPropertyListStyle:
            return true;
        case kCSSPropertyListStyleImage:
            return true;
        case kCSSPropertyListStyleImageBaseurl:
            return false;
        case kCSSPropertyListStylePosition:
            return true;
        case kCSSPropertyListStyleType:
            return true;
        case kCSSPropertyMargin:
            return true;
        case kCSSPropertyMarginBottom:
            return false;
        case kCSSPropertyMarginLeft:
            return false;
        case kCSSPropertyMarginRight:
            return false;
        case kCSSPropertyMarginTop:
            return false;
        case kCSSPropertyMaxHeight:
            return false;
        case kCSSPropertyMaxWidth:
            return false;
        case kCSSPropertyMinHeight:
            return false;
        case kCSSPropertyMinWidth:
            return false;
        case kCSSPropertyOverflow:
            return false;
        case kCSSPropertyPadding:
            return true;
        case kCSSPropertyPaddingBottom:
            return false;
        case kCSSPropertyPaddingLeft:
            return false;
        case kCSSPropertyPaddingRight:
            return false;
        case kCSSPropertyPaddingTop:
            return false;
        case kCSSPropertyPosition:
            return false;
        case kCSSPropertyRight:
            return false;
        case kCSSPropertyTextAlign:
            return true;
        case kCSSPropertyTextDecoration:
            return false;
        case kCSSPropertyTextIndent:
            return true;
        case kCSSPropertyTextShadow:
            return true;
        case kCSSPropertyTextTransform:
            return true;
        case kCSSPropertyTop:
            return false;
        case kCSSPropertyVerticalAlign:
            return true;
        case kCSSPropertyVisibility:
            return true;
        case kCSSPropertyWhiteSpace:
            return true;
        case kCSSPropertyWidth:
            return false;
        case kCSSPropertyZIndex:
            return false;
        case kCSSPropertyUnknown:
        default:
            return false;
    }
}

CSSValueType css_property_value_type(CSSProperty property)
{
    switch (property) {
        case kCSSPropertyNone:
            return kCSSValueString;
        case kCSSPropertyLitehtmlBorderSpacingX:
            return kCSSValueString;
        case kCSSPropertyLitehtmlBorderSpacingY:
            return kCSSValueString;
        case kCSSPropertyBackground:
            return kCSSValueString;
        case kCSSPropertyBackgroundAttachment:
            return kCSSValueKeyword;
        case kCSSPropertyBackgroundClip:
            return kCSSValueKeyword;
        case kCSSPropertyBackgroundColor:
            return kCSSValueColor;
        case kCSSPropertyBackgroundImage:
            return kCSSValueString;
        case kCSSPropertyBackgroundImageBaseurl:
            return kCSSValueString;
        case kCSSPropertyBackgroundOrigin:
            return kCSSValueKeyword;
        case kCSSPropertyBackgroundPosition:
            return kCSSValueString;
        case kCSSPropertyBackgroundRepeat:
            return kCSSValueKeyword;
        case kCSSPropertyBackgroundSize:
            return kCSSValueString;
        case kCSSPropertyBorder:
            return kCSSValueString;
        case kCSSPropertyBorderBottom:
            return kCSSValueString;
        case kCSSPropertyBorderBottomColor:
            return kCSSValueColor;
        case kCSSPropertyBorderBottomLeftRadius:
            return kCSSValueString;
        case kCSSPropertyBorderBottomLeftRadiusX:
            return kCSSValueLength;
        case kCSSPropertyBorderBottomLeftRadiusY:
            return kCSSValueLength;
        case kCSSPropertyBorderBottomRightRadius:
            return kCSSValueString;
        case kCSSPropertyBorderBottomRightRadiusX:
            return kCSSValueLength;
        case kCSSPropertyBorderBottomRightRadiusY:
            return kCSSValueLength;
        case kCSSPropertyBorderBottomStyle:
            return kCSSValueKeyword;
        case kCSSPropertyBorderBottomWidth:
            return kCSSValueLength;
        case kCSSPropertyBorderCollapse:
            return kCSSValueString;
        case kCSSPropertyBorderColor:
            return kCSSValueString;
        case kCSSPropertyBorderLeft:
            return kCSSValueString;
        case kCSSPropertyBorderLeftColor:
            return kCSSValueColor;
        case kCSSPropertyBorderLeftStyle:
            return kCSSValueKeyword;
        case kCSSPropertyBorderLeftWidth:
            return kCSSValueLength;
        case kCSSPropertyBorderRadius:
            return kCSSValueString;
        case kCSSPropertyBorderRadiusX:
            return kCSSValueLength;
        case kCSSPropertyBorderRadiusY:
            return kCSSValueLength;
        case kCSSPropertyBorderRight:
            return kCSSValueString;
        case kCSSPropertyBorderRightColor:
            return kCSSValueColor;
        case kCSSPropertyBorderRightStyle:
            return kCSSValueKeyword;
        case kCSSPropertyBorderRightWidth:
            return kCSSValueLength;
        case kCSSPropertyBorderSpacing:
            return kCSSValueString;
        case kCSSPropertyBorderStyle:
            return kCSSValueString;
        case kCSSPropertyBorderTop:
            return kCSSValueString;
        case kCSSPropertyBorderTopColor:
            return kCSSValueColor;
        case kCSSPropertyBorderTopLeftRadius:
            return kCSSValueString;
        case kCSSPropertyBorderTopLeftRadiusX:
            return kCSSValueLength;
        case kCSSPropertyBorderTopLeftRadiusY:
            return kCSSValueLength;
        case kCSSPropertyBorderTopRightRadius:
            return kCSSValueString;
        case kCSSPropertyBorderTopRightRadiusX:
            return kCSSValueLength;
        case kCSSPropertyBorderTopRightRadiusY:
            return kCSSValueLength;
        case kCSSPropertyBorderTopStyle:
            return kCSSValueKeyword;
        case kCSSPropertyBorderTopWidth:
            return kCSSValueLength;
        case kCSSPropertyBorderWidth:
            return kCSSValueString;
        case kCSSPropertyBottom:
            return kCSSValueLength;
        case kCSSPropertyBoxSizing:
            return kCSSValueKeyword;
        case kCSSPropertyClear:
            return kCSSValueKeyword;
        case kCSSPropertyColor:
            return kCSSValueColor;
        case kCSSPropertyContent:
            return kCSSValueString;
        case kCSSPropertyCursor:
            return kCSSValueString;
        case kCSSPropertyDisplay:
            return kCSSValueKeyword;
        case kCSSPropertyFloat:
            return kCSSValueKeyword;
        case kCSSPropertyFont:
            return kCSSValueString;
        case kCSSPropertyFontFamily:
            return kCSSValueString;
        case kCSSPropertyFontSize:
            return kCSSValueString;
        case kCSSPropertyFontStyle:
            return kCSSValueString;
        case kCSSPropertyFontVariant:
            return kCSSValueString;
        case kCSSPropertyFontWeight:
            return kCSSValueString;
        case kCSSPropertyHeight:
            return kCSSValueLength;
        case kCSSPropertyLeft:
            return kCSSValueLength;
        case kCSSPropertyLineHeight:
            return kCSSValueLength;
        case kCSSPropertyListStyle:
            return kCSSValueString;
        case kCSSPropertyListStyleImage:
            return kCSSValueString;
        case kCSSPropertyListStyleImageBaseurl:
            return kCSSValueString;
        case kCSSPropertyListStylePosition:
            return kCSSValueKeyword;
        case kCSSPropertyListStyleType:
            return kCSSValueKeyword;
        case kCSSPropertyMargin:
            return kCSSValueString;
        case kCSSPropertyMarginBottom:
            return kCSSValueLength;
        case kCSSPropertyMarginLeft:
            return kCSSValueLength;
        case kCSSPropertyMarginRight:
            return kCSSValueLength;
        case kCSSPropertyMarginTop:
            return kCSSValueLength;
        case kCSSPropertyMaxHeight:
            return kCSSValueLength;
        case kCSSPropertyMaxWidth:
            return kCSSValueLength;
        case kCSSPropertyMinHeight:
            return kCSSValueLength;
        case kCSSPropertyMinWidth:
            return kCSSValueLength;
        case kCSSPropertyOverflow:
            return kCSSValueKeyword;
        case kCSSPropertyPadding:
            return kCSSValueString;
        case kCSSPropertyPaddingBottom:
            return kCSSValueLength;
        case kCSSPropertyPaddingLeft:
            return kCSSValueLength;
        case kCSSPropertyPaddingRight:
            return kCSSValueLength;
        case kCSSPropertyPaddingTop:
            return kCSSValueLength;
        case kCSSPropertyPosition:
            return kCSSValueKeyword;
        case kCSSPropertyRight:
            return kCSSValueLength;
        case kCSSPropertyTextAlign:
            return kCSSValueKeyword;
        case kCSSPropertyTextDecoration:
            return kCSSValueString;
        case kCSSPropertyTextIndent:
            return kCSSValueLength;
        case kCSSPropertyTextShadow:
            return kCSSValueString;
        case kCSSPropertyTextTransform:
            return kCSSValueString;
        case kCSSPropertyTop:
            return kCSSValueLength;
        case kCSSPropertyVerticalAlign:
            return kCSSValueKeyword;
        case kCSSPropertyVisibility:
            return kCSSValueKeyword;
        case kCSSPropertyWhiteSpace:
            return kCSSValueKeyword;
        case kCSSPropertyWidth:
            return kCSSValueLength;
        case kCSSPropertyZIndex:
            return kCSSValueString;
        case kCSSPropertyUnknown:
        default:
            return kCSSValueString;
    }
}

const KeywordVector& css_property_keywords(CSSProperty property)
{
    switch (property) {
        case kCSSPropertyNone: {
            static const KeywordVector keywords;
            return keywords;
        }
        case kCSSPropertyLitehtmlBorderSpacingX: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyLitehtmlBorderSpacingY: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBackground: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBackgroundAttachment: {
            static const KeywordVector keywords = {
                { "scroll", kBackgroundAttachmentScroll },
                { "fixed", kBackgroundAttachmentFixed },
            };
            return keywords;
        }
        case kCSSPropertyBackgroundClip: {
            static const KeywordVector keywords = {
                { "border-box", kBackgroundBoxBorderBox },
                { "padding-box", kBackgroundBoxPaddingBox },
                { "content-box", kBackgroundBoxContentBox },
            };
            return keywords;
        }
        case kCSSPropertyBackgroundColor: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBackgroundImage: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBackgroundImageBaseurl: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBackgroundOrigin: {
            static const KeywordVector keywords = {
                { "border-box", kBackgroundBoxBorderBox },
                { "padding-box", kBackgroundBoxPaddingBox },
                { "content-box", kBackgroundBoxContentBox },
            };
            return keywords;
        }
        case kCSSPropertyBackgroundPosition: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBackgroundRepeat: {
            static const KeywordVector keywords = {
                { "repeat", kBackgroundRepeatRepeat },
                { "repeat-x", kBackgroundRepeatRepeatX },
                { "repeat-y", kBackgroundRepeatRepeatY },
                { "no-repeat", kBackgroundRepeatNoRepeat },
            };
            return keywords;
        }
        case kCSSPropertyBackgroundSize: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorder: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderBottom: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderBottomColor: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderBottomLeftRadius: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderBottomLeftRadiusX: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderBottomLeftRadiusY: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderBottomRightRadius: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderBottomRightRadiusX: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderBottomRightRadiusY: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderBottomStyle: {
            static const KeywordVector keywords = {
                { "none", kBorderStyleNone },
                { "hidden", kBorderStyleHidden },
                { "dotted", kBorderStyleDotted },
                { "dashed", kBorderStyleDashed },
                { "solid", kBorderStyleSolid },
                { "double", kBorderStyleDouble },
                { "groove", kBorderStyleGroove },
                { "ridge", kBorderStyleRidge },
                { "inset", kBorderStyleInset },
                { "outset", kBorderStyleOutset },
            };
            return keywords;
        }
        case kCSSPropertyBorderBottomWidth: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderCollapse: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderColor: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderLeft: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderLeftColor: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderLeftStyle: {
            static const KeywordVector keywords = {
                { "none", kBorderStyleNone },
                { "hidden", kBorderStyleHidden },
                { "dotted", kBorderStyleDotted },
                { "dashed", kBorderStyleDashed },
                { "solid", kBorderStyleSolid },
                { "double", kBorderStyleDouble },
                { "groove", kBorderStyleGroove },
                { "ridge", kBorderStyleRidge },
                { "inset", kBorderStyleInset },
                { "outset", kBorderStyleOutset },
            };
            return keywords;
        }
        case kCSSPropertyBorderLeftWidth: {
            static const KeywordVector keywords = {
                { "thin", kLengthBorderWidthThin },
                { "medium", kLengthBorderWidthMedium },
                { "thick", kLengthBorderWidthThick },
            };
            return keywords;
        }
        case kCSSPropertyBorderRadius: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderRadiusX: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderRadiusY: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderRight: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderRightColor: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderRightStyle: {
            static const KeywordVector keywords = {
                { "none", kBorderStyleNone },
                { "hidden", kBorderStyleHidden },
                { "dotted", kBorderStyleDotted },
                { "dashed", kBorderStyleDashed },
                { "solid", kBorderStyleSolid },
                { "double", kBorderStyleDouble },
                { "groove", kBorderStyleGroove },
                { "ridge", kBorderStyleRidge },
                { "inset", kBorderStyleInset },
                { "outset", kBorderStyleOutset },
            };
            return keywords;
        }
        case kCSSPropertyBorderRightWidth: {
            static const KeywordVector keywords = {
                { "thin", kLengthBorderWidthThin },
                { "medium", kLengthBorderWidthMedium },
                { "thick", kLengthBorderWidthThick },
            };
            return keywords;
        }
        case kCSSPropertyBorderSpacing: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderStyle: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderTop: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderTopColor: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderTopLeftRadius: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderTopLeftRadiusX: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderTopLeftRadiusY: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderTopRightRadius: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderTopRightRadiusX: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderTopRightRadiusY: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBorderTopStyle: {
            static const KeywordVector keywords = {
                { "none", kBorderStyleNone },
                { "hidden", kBorderStyleHidden },
                { "dotted", kBorderStyleDotted },
                { "dashed", kBorderStyleDashed },
                { "solid", kBorderStyleSolid },
                { "double", kBorderStyleDouble },
                { "groove", kBorderStyleGroove },
                { "ridge", kBorderStyleRidge },
                { "inset", kBorderStyleInset },
                { "outset", kBorderStyleOutset },
            };
            return keywords;
        }
        case kCSSPropertyBorderTopWidth: {
            static const KeywordVector keywords = {
                { "thin", kLengthBorderWidthThin },
                { "medium", kLengthBorderWidthMedium },
                { "thick", kLengthBorderWidthThick },
            };
            return keywords;
        }
        case kCSSPropertyBorderWidth: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyBottom: {
            static const KeywordVector keywords = {
                { "auto", kLengthAuto },
                { "inherit", kLengthInherit },
            };
            return keywords;
        }
        case kCSSPropertyBoxSizing: {
            static const KeywordVector keywords = {
                { "content-box", kBoxSizingContentBox },
                { "border-box", kBoxSizingBorderBox },
            };
            return keywords;
        }
        case kCSSPropertyClear: {
            static const KeywordVector keywords = {
                { "none", kClearNone },
                { "left", kClearLeft },
                { "right", kClearRight },
                { "both", kClearBoth },
            };
            return keywords;
        }
        case kCSSPropertyColor: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyContent: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyCursor: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyDisplay: {
            static const KeywordVector keywords = {
                { "none", kDisplayNone },
                { "block", kDisplayBlock },
                { "inline", kDisplayInline },
                { "inline-block", kDisplayInlineBlock },
                { "inline-table", kDisplayInlineTable },
                { "list-item", kDisplayListItem },
                { "table", kDisplayTable },
                { "table-caption", kDisplayTableCaption },
                { "table-cell", kDisplayTableCell },
                { "table-column", kDisplayTableColumn },
                { "table-column-group", kDisplayTableColumnGroup },
                { "table-footer-group", kDisplayTableFooterGroup },
                { "table-header-group", kDisplayTableHeaderGroup },
                { "table-row", kDisplayTableRow },
                { "table-row-group", kDisplayTableRowGroup },
                { "inline-text", kDisplayInlineText },
            };
            return keywords;
        }
        case kCSSPropertyFloat: {
            static const KeywordVector keywords = {
                { "none", kFloatNone },
                { "left", kFloatLeft },
                { "right", kFloatRight },
            };
            return keywords;
        }
        case kCSSPropertyFont: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyFontFamily: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyFontSize: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyFontStyle: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyFontVariant: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyFontWeight: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyHeight: {
            static const KeywordVector keywords = {
                { "auto", kLengthAuto },
            };
            return keywords;
        }
        case kCSSPropertyLeft: {
            static const KeywordVector keywords = {
                { "auto", kLengthAuto },
                { "inherit", kLengthInherit },
            };
            return keywords;
        }
        case kCSSPropertyLineHeight: {
            static const KeywordVector keywords = {
                { "normal", kLengthNormal },
            };
            return keywords;
        }
        case kCSSPropertyListStyle: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyListStyleImage: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyListStyleImageBaseurl: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyListStylePosition: {
            static const KeywordVector keywords = {
                { "inside", kListStylePositionInside },
                { "outside", kListStylePositionOutside },
            };
            return keywords;
        }
        case kCSSPropertyListStyleType: {
            static const KeywordVector keywords = {
                { "none", kListStyleTypeNone },
                { "circle", kListStyleTypeCircle },
                { "disc", kListStyleTypeDisc },
                { "square", kListStyleTypeSquare },
                { "armenian", kListStyleTypeArmenian },
                { "cjk-ideographic", kListStyleTypeCjkIdeographic },
                { "decimal", kListStyleTypeDecimal },
                { "decimal-leading-zero", kListStyleTypeDecimalLeadingZero },
                { "georgian", kListStyleTypeGeorgian },
                { "hebrew", kListStyleTypeHebrew },
                { "hiragana", kListStyleTypeHiragana },
                { "hiragana-iroha", kListStyleTypeHiraganaIroha },
                { "katakana", kListStyleTypeKatakana },
                { "katakana-iroha", kListStyleTypeKatakanaIroha },
                { "lower-alpha", kListStyleTypeLowerAlpha },
                { "lower-greek", kListStyleTypeLowerGreek },
                { "lower-latin", kListStyleTypeLowerLatin },
                { "lower-roman", kListStyleTypeLowerRoman },
                { "upper-alpha", kListStyleTypeUpperAlpha },
                { "upper-latin", kListStyleTypeUpperLatin },
                { "upper-roman", kListStyleTypeUpperRoman },
            };
            return keywords;
        }
        case kCSSPropertyMargin: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyMarginBottom: {
            static const KeywordVector keywords = {
                { "auto", kLengthAuto },
            };
            return keywords;
        }
        case kCSSPropertyMarginLeft: {
            static const KeywordVector keywords = {
                { "auto", kLengthAuto },
            };
            return keywords;
        }
        case kCSSPropertyMarginRight: {
            static const KeywordVector keywords = {
                { "auto", kLengthAuto },
            };
            return keywords;
        }
        case kCSSPropertyMarginTop: {
            static const KeywordVector keywords = {
                { "auto", kLengthAuto },
            };
            return keywords;
        }
        case kCSSPropertyMaxHeight: {
            static const KeywordVector keywords = {
                { "none", kLengthNone },
            };
            return keywords;
        }
        case kCSSPropertyMaxWidth: {
            static const KeywordVector keywords = {
                { "none", kLengthNone },
            };
            return keywords;
        }
        case kCSSPropertyMinHeight: {
            static const KeywordVector keywords = {
                { "auto", kLengthAuto },
            };
            return keywords;
        }
        case kCSSPropertyMinWidth: {
            static const KeywordVector keywords = {
                { "auto", kLengthAuto },
            };
            return keywords;
        }
        case kCSSPropertyOverflow: {
            static const KeywordVector keywords = {
                { "visible", kOverflowVisible },
                { "hidden", kOverflowHidden },
                { "scroll", kOverflowScroll },
                { "auto", kOverflowAuto },
                { "no-display", kOverflowNoDisplay },
                { "no-content", kOverflowNoContent },
            };
            return keywords;
        }
        case kCSSPropertyPadding: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyPaddingBottom: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyPaddingLeft: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyPaddingRight: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyPaddingTop: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyPosition: {
            static const KeywordVector keywords = {
                { "static", kPositionStatic },
                { "relative", kPositionRelative },
                { "absolute", kPositionAbsolute },
                { "fixed", kPositionFixed },
            };
            return keywords;
        }
        case kCSSPropertyRight: {
            static const KeywordVector keywords = {
                { "auto", kLengthAuto },
                { "inherit", kLengthInherit },
            };
            return keywords;
        }
        case kCSSPropertyTextAlign: {
            static const KeywordVector keywords = {
                { "left", kTextAlignLeft },
                { "right", kTextAlignRight },
                { "center", kTextAlignCenter },
                { "justify", kTextAlignJustify },
            };
            return keywords;
        }
        case kCSSPropertyTextDecoration: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyTextIndent: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyTextShadow: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyTextTransform: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyTop: {
            static const KeywordVector keywords = {
                { "auto", kLengthAuto },
                { "inherit", kLengthInherit },
            };
            return keywords;
        }
        case kCSSPropertyVerticalAlign: {
            static const KeywordVector keywords = {
                { "baseline", kVerticalAlignBaseline },
                { "sub", kVerticalAlignSub },
                { "super", kVerticalAlignSuper },
                { "top", kVerticalAlignTop },
                { "text-top", kVerticalAlignTextTop },
                { "middle", kVerticalAlignMiddle },
                { "bottom", kVerticalAlignBottom },
                { "text-bottom", kVerticalAlignTextBottom },
            };
            return keywords;
        }
        case kCSSPropertyVisibility: {
            static const KeywordVector keywords = {
                { "visible", kVisibilityVisible },
                { "hidden", kVisibilityHidden },
                { "collapse", kVisibilityCollapse },
            };
            return keywords;
        }
        case kCSSPropertyWhiteSpace: {
            static const KeywordVector keywords = {
                { "normal", kWhiteSpaceNormal },
                { "nowrap", kWhiteSpaceNowrap },
                { "pre", kWhiteSpacePre },
                { "pre-line", kWhiteSpacePreLine },
                { "pre-wrap", kWhiteSpacePreWrap },
            };
            return keywords;
        }
        case kCSSPropertyWidth: {
            static const KeywordVector keywords = {
                { "auto", kLengthAuto },
            };
            return keywords;
        }
        case kCSSPropertyZIndex: {
            static const KeywordVector keywords = {
            };
            return keywords;
        }
        case kCSSPropertyUnknown:
        default: {
            static const KeywordVector keywords;
            return keywords;
        }
    }
}

} // namespace litehtml
