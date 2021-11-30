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

#ifndef LITEHTML_TYPES_H__
#define LITEHTML_TYPES_H__
#include <stdlib.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "litehtml/string.h"

namespace litehtml {

class Document;
class Element;

typedef std::map<litehtml::tstring, litehtml::tstring> string_map;
typedef std::vector<Element*> ElementsVector;
typedef std::vector<int> int_vector;
typedef std::vector<litehtml::tstring> string_vector;

const unsigned int font_decoration_none = 0x00;
const unsigned int font_decoration_underline = 0x01;
const unsigned int font_decoration_linethrough = 0x02;
const unsigned int font_decoration_overline = 0x04;

typedef unsigned char byte;
typedef unsigned int ucode_t;

struct margins {
    int left;
    int right;
    int top;
    int bottom;

    margins()
    {
        left = right = top = bottom = 0;
    }

    int width() const
    {
        return left + right;
    }
    int height() const
    {
        return top + bottom;
    }
};

struct Size {
    int width = 0;
    int height = 0;

    Size() = default;

    Size(int w, int h)
    : width(w)
    , height(h)
    {
    }
};

struct Position {
    typedef std::vector<Position> vector;

    int x;
    int y;
    int width;
    int height;

    Position()
    {
        x = y = width = height = 0;
    }

    Position(int x, int y, int width, int height)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    int right() const
    {
        return x + width;
    }

    int bottom() const
    {
        return y + height;
    }

    int left() const
    {
        return x;
    }

    int top() const
    {
        return y;
    }

    void operator+=(const margins& mg)
    {
        x -= mg.left;
        y -= mg.top;
        width += mg.left + mg.right;
        height += mg.top + mg.bottom;
    }

    void operator-=(const margins& mg)
    {
        x += mg.left;
        y += mg.top;
        width -= mg.left + mg.right;
        height -= mg.top + mg.bottom;
    }

    void clear()
    {
        x = y = width = height = 0;
    }

    void operator=(const Size& sz)
    {
        width = sz.width;
        height = sz.height;
    }

    void move_to(int new_x, int new_y)
    {
        x = new_x;
        y = new_y;
    }

    bool does_intersect(const Position* val) const
    {
        if (!val)
            return true;

        return (left() <= val->right() && right() >= val->left() &&
                   bottom() >= val->top() && top() <= val->bottom()) ||
               (val->left() <= right() && val->right() >= left() &&
                   val->bottom() >= top() && val->top() <= bottom());
    }

    bool empty() const
    {
        if (!width && !height) {
            return true;
        }
        return false;
    }

    bool is_point_inside(int other_x, int other_y) const
    {
        if (other_x >= left() && other_x <= right() && other_y >= top() && other_y <= bottom()) {
            return true;
        }
        return false;
    }
};

struct FontMetrics {
    int height;
    int ascent;
    int descent;
    int x_height;
    bool draw_spaces;

    FontMetrics()
    {
        height = 0;
        ascent = 0;
        descent = 0;
        x_height = 0;
        draw_spaces = true;
    }

    int baseline()
    {
        return descent;
    }
};

struct FontItem {
    uintptr_t font;
    FontMetrics metrics;
};

typedef std::map<tstring, FontItem> FontMap;

enum DrawFlag {
    kDrawRoot,
    kDrawBlock,
    kDrawFloats,
    kDrawInlines,
    kDrawPositioned,
};

enum Display {
    kDisplayNone,
    kDisplayBlock,
    kDisplayInline,
    kDisplayInlineBlock,
    kDisplayInlineTable,
    kDisplayListItem,
    kDisplayTable,
    kDisplayTableCaption,
    kDisplayTableCell,
    kDisplayTableColumn,
    kDisplayTableColumnGroup,
    kDisplayTableFooterGroup,
    kDisplayTableHeaderGroup,
    kDisplayTableRow,
    kDisplayTableRowGroup,
    kDisplayInlineText,
};

enum StyleBorder {
    kStyleBorderNone,
    kStyleBorderHidden,
    kStyleBorderDotted,
    kStyleBorderDashed,
    kStyleBorderSolid,
    kStyleBorderDouble
};

enum Length {
    kLengthNone,
    kLengthAuto,
    kLengthInherit,
    kLengthNormal,
    kLengthBorderWidthThin,
    kLengthBorderWidthMedium,
    kLengthBorderWidthThick,
};

#define FONT_SIZE_STRINGS \
    _t("xx-small;x-small;small;medium;large;x-large;xx-large;smaller;larger")

enum FontSize {
    kFontSizeXXSmall,
    kFontSizeXSmall,
    kFontSizeSmall,
    kFontSizeMedium,
    kFontSizeLarge,
    kFontSizeXLarge,
    kFontSizeXXLarge,
    kFontSizeSmaller,
    kFontSizeLarger,
};

#define font_style_strings _t("normal;italic")

enum font_style { fontStyleNormal, fontStyleItalic };

#define font_variant_strings _t("normal;small-caps")

enum font_variant { font_variant_normal, font_variant_italic };

#define FONT_WEIGHT_STRINGS \
    _t("normal;bold;bolder;lighter;100;200;300;400;500;600;700")

enum FontWeight {
    kFontWeightNormal,
    kFontWeightBold,
    kFontWeightBolder,
    kFontWeightLighter,
    kFontWeight100,
    kFontWeight200,
    kFontWeight300,
    kFontWeight400,
    kFontWeight500,
    kFontWeight600,
    kFontWeight700
};

#define LIST_STYLE_TYPE_STRINGS                                                \
    _t("none;circle;disc;square;armenian;cjk-ideographic;decimal;decimal-"     \
       "leading-zero;georgian;hebrew;hiragana;hiragana-iroha;katakana;"        \
       "katakana-iroha;lower-alpha;lower-greek;lower-latin;lower-roman;upper-" \
       "alpha;upper-latin;upper-roman")

enum ListStyleType {
    kListStyleTypeNone,
    kListStyleTypeCircle,
    kListStyleTypeDisc,
    kListStyleTypeSquare,
    kListStyleTypeArmenian,
    kListStyleTypeCjkIdeographic,
    kListStyleTypeDecimal,
    kListStyleTypeDecimalLeadingZero,
    kListStyleTypeGeorgian,
    kListStyleTypeHebrew,
    kListStyleTypeHiragana,
    kListStyleTypeHiraganaIroha,
    kListStyleTypeKatakana,
    kListStyleTypeKatakanaIroha,
    kListStyleTypeLowerAlpha,
    kListStyleTypeLowerGreek,
    kListStyleTypeLowerLatin,
    kListStyleTypeLowerRoman,
    kListStyleTypeUpperAlpha,
    kListStyleTypeUpperLatin,
    kListStyleTypeUpperRoman,
};

#define LIST_STYLE_POSITION_STRINGS _t("inside;outside")

enum ListStylePosition {
    kListStylePositionInside,
    kListStylePositionOutside
};

#define VerticalAlign_strings \
    _t("baseline;sub;super;top;text-top;middle;bottom;text-bottom")

enum VerticalAlign {
    kVerticalAlignBaseline,
    kVerticalAlignSub,
    kVerticalAlignSuper,
    kVerticalAlignTop,
    kVerticalAlignTextTop,
    kVerticalAlignMiddle,
    kVerticalAlignBottom,
    kVerticalAlignTextBottom
};

#define border_width_strings _t("thin;medium;thick")

enum border_width {
    border_width_thin,
    border_width_medium,
    border_width_thick
};

#define BORDER_STYLE_STRINGS \
    _t("none;hidden;dotted;dashed;solid;double;groove;ridge;inset;outset")

enum BorderStyle {
    kBorderStyleNone,
    kBorderStyleHidden,
    kBorderStyleDotted,
    kBorderStyleDashed,
    kBorderStyleSolid,
    kBorderStyleDouble,
    kBorderStyleGroove,
    kBorderStyleRidge,
    kBorderStyleInset,
    kBorderStyleOutset
};

enum ElementFloat {
    kFloatNone,
    kFloatLeft,
    kFloatRight
};

enum ElementClear {
    kClearNone,
    kClearLeft,
    kClearRight,
    kClearBoth
};

#define CSS_UNITS_STRINGS \
    _t("none;%;in;cm;mm;em;ex;pt;pc;px;dpi;dpcm;vw;vh;vmin;vmax;rem")

enum CSSUnits {
    kCSSUnitsNone,
    kCSSUnitsPercent,
    kCSSUnitsIn,
    kCSSUnitsCm,
    kCSSUnitsMm,
    kCSSUnitsEm,
    kCSSUnitsEx,
    kCSSUnitsPt,
    kCSSUnitsPc,
    kCSSUnitsPx,
    kCSSUnitsDpi,
    kCSSUnitsDpcm,
    kCSSUnitsVw,
    kCSSUnitsVh,
    kCSSUnitsVmin,
    kCSSUnitsVmax,
    kCSSUnitsRem,
};

enum ElementPosition {
    kPositionStatic,
    kPositionRelative,
    kPositionAbsolute,
    kPositionFixed,
};

enum TextAlign {
    kTextAlignLeft,
    kTextAlignRight,
    kTextAlignCenter,
    kTextAlignJustify
};

#define TEXT_TRANSFORM_STRINGS _t("none;capitalize;uppercase;lowercase")

enum TextTransform {
    kTextTransformNone,
    kTextTransformCapitalize,
    kTextTransformUppercase,
    kTextTransformLowercase
};

enum WhiteSpace {
    kWhiteSpaceNormal,
    kWhiteSpaceNowrap,
    kWhiteSpacePre,
    kWhiteSpacePreLine,
    kWhiteSpacePreWrap
};

enum Overflow {
    kOverflowVisible,
    kOverflowHidden,
    kOverflowScroll,
    kOverflowAuto,
    kOverflowNoDisplay,
    kOverflowNoContent
};

#define BACKGROUND_SIZE_STRINGS _t("auto;cover;contain")

enum background_size {
    background_size_auto,
    background_size_cover,
    background_size_contain,
};

enum Visibility {
    kVisibilityVisible,
    kVisibilityHidden,
    kVisibilityCollapse,
};

#define BORDER_COLLAPSE_STRINGS _t("collapse;separate")

enum border_collapse {
    border_collapse_collapse,
    border_collapse_separate,
};


#define PSEUDO_CLASS_STRINGS                                                   \
    _t("only-child;only-of-type;first-child;first-of-type;last-child;last-of-" \
       "type;nth-child;nth-of-type;nth-last-child;nth-last-of-type;not;lang")

enum pseudo_class {
    pseudo_class_only_child,
    pseudo_class_only_of_type,
    pseudo_class_first_child,
    pseudo_class_first_of_type,
    pseudo_class_last_child,
    pseudo_class_last_of_type,
    pseudo_class_nth_child,
    pseudo_class_nth_of_type,
    pseudo_class_nth_last_child,
    pseudo_class_nth_last_of_type,
    pseudo_class_not,
    pseudo_class_lang,
};

#define CONTENT_PROPERTY_STRING \
    _t("none;normal;open-quote;close-quote;no-open-quote;no-close-quote")

enum content_property {
    content_property_none,
    content_property_normal,
    content_property_open_quote,
    content_property_close_quote,
    content_property_no_open_quote,
    content_property_no_close_quote,
};


struct floated_box {
    typedef std::vector<floated_box> vector;

    Position pos;
    ElementFloat float_side;
    ElementClear clear_floats;
    Element* el;

    floated_box() = default;
    floated_box(const floated_box& val)
    {
        pos = val.pos;
        float_side = val.float_side;
        clear_floats = val.clear_floats;
        el = val.el;
    }
    floated_box& operator=(const floated_box& val)
    {
        pos = val.pos;
        float_side = val.float_side;
        clear_floats = val.clear_floats;
        el = val.el;
        return *this;
    }
    floated_box(floated_box&& val)
    {
        pos = val.pos;
        float_side = val.float_side;
        clear_floats = val.clear_floats;
        el = std::move(val.el);
    }
    void operator=(floated_box&& val)
    {
        pos = val.pos;
        float_side = val.float_side;
        clear_floats = val.clear_floats;
        el = std::move(val.el);
    }
};

struct int_int_cache {
    int hash;
    int val;
    bool is_valid;
    bool is_default;

    int_int_cache()
    {
        hash = 0;
        val = 0;
        is_valid = false;
        is_default = false;
    }
    void invalidate()
    {
        is_valid = false;
        is_default = false;
    }
    void set_value(int vHash, int vVal)
    {
        hash = vHash;
        val = vVal;
        is_valid = true;
    }
};

enum select_result {
    select_no_match = 0x00,
    select_match = 0x01,
    select_match_pseudo_class = 0x02,
    select_match_with_before = 0x10,
    select_match_with_after = 0x20,
};

template <class T>
class def_value {
    T m_val;
    bool m_is_default;

public:
    def_value(T def_val)
    {
        m_is_default = true;
        m_val = def_val;
    }
    void reset(T def_val)
    {
        m_is_default = true;
        m_val = def_val;
    }
    bool is_default()
    {
        return m_is_default;
    }
    T operator=(T new_val)
    {
        m_val = new_val;
        m_is_default = false;
        return m_val;
    }
    operator T()
    {
        return m_val;
    }
};

enum BoxSizing {
    kBoxSizingContentBox,
    kBoxSizingBorderBox,
};

enum RenderType {
    kRenderAll,
    kRenderNoFixed,
    kRenderFixedOnly,
};

// List of the Void Elements (can't have any contents)
const litehtml::tchar_t* const void_elements =
    _t("area;base;br;col;command;embed;hr;img;input;keygen;link;meta;param;"
       "source;track;wbr");

} // namespace litehtml

#endif // LITEHTML_TYPES_H__
