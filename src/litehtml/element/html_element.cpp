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

#include "litehtml/element/html_element.h"

#include <algorithm>
#include <locale>

#include "litehtml/css/css_stylesheet.h"
#include "litehtml/document.h"
#include "litehtml/document_container.h"
#include "litehtml/element/before_after_element.h"
#include "litehtml/html.h"
#include "litehtml/iterators.h"
#include "litehtml/logging.h"
#include "litehtml/num_cvt.h"
#include "litehtml/table.h"

namespace litehtml {

HTMLElement::HTMLElement(Document* doc)
: Element(doc)
{
    box_sizing_ = kBoxSizingContentBox;
    m_z_index = 0;
    overflow_ = kOverflowVisible;
    m_box = nullptr;
    m_text_align = kTextAlignLeft;
    m_el_position = kPositionStatic;
    m_display = kDisplayInline;
    vertical_align_ = kVerticalAlignBaseline;
    list_style_type_ = kListStyleTypeNone;
    list_style_position_ = kListStylePositionOutside;
    m_float = kFloatNone;
    m_clear = kClearNone;
    font_ = 0;
    font_size_ = 0;
    white_space_ = kWhiteSpaceNormal;
    m_lh_predefined = false;
    line_height_ = 0;
    m_visibility = kVisibilityVisible;
    m_border_spacing_x = 0;
    m_border_spacing_y = 0;
    m_border_collapse = border_collapse_separate;
}

HTMLElement::~HTMLElement()
{
}

bool HTMLElement::append_child(Element* element)
{
    if (element) {
        element->parent(this);
        m_children.push_back(element);
        return true;
    }
    return false;
}

const tchar_t* HTMLElement::get_tagName() const
{
    return m_tag.c_str();
}

void HTMLElement::set_attr(const tchar_t* name, const tchar_t* val)
{
    if (name && val) {
        tstring s_val = name;
        for (size_t i = 0; i < s_val.length(); i++) {
            s_val[i] = std::tolower(s_val[i], std::locale::classic());
        }
        m_attrs[s_val] = val;

        if (t_strcasecmp(name, _t("class")) == 0) {
            m_class_values.resize(0);
            split_string(val, m_class_values, _t(" "));
        }
    }
}

const tchar_t* HTMLElement::get_attr(const tchar_t* name, const tchar_t* def) const
{
    string_map::const_iterator attr = m_attrs.find(name);
    if (attr != m_attrs.end()) {
        return attr->second.c_str();
    }
    return def;
}

ElementsVector HTMLElement::select_all(const tstring& selector)
{
    CSSSelector sel(MediaQueryList::ptr(nullptr));
    sel.parse(selector);

    return select_all(sel);
}

ElementsVector HTMLElement::select_all(const CSSSelector& selector)
{
    ElementsVector res;
    select_all(selector, res);
    return res;
}

void HTMLElement::select_all(const CSSSelector& selector, ElementsVector& res)
{
    if (select(selector)) {
        res.push_back(this);
    }

    for (auto& el : m_children) {
        el->select_all(selector, res);
    }
}


Element::ptr HTMLElement::select_one(const tstring& selector)
{
    CSSSelector sel(MediaQueryList::ptr(nullptr));
    sel.parse(selector);

    return select_one(sel);
}

Element::ptr HTMLElement::select_one(const CSSSelector& selector)
{
    if (select(selector)) {
        return this;
    }

    for (auto& el : m_children) {
        Element::ptr res = el->select_one(selector);
        if (res) {
            return res;
        }
    }
    return nullptr;
}

void HTMLElement::apply_stylesheet(const CSSStylesheet& stylesheet)
{
    remove_before_after();

    for (const auto& sel : stylesheet.selectors()) {
        int apply = select(*sel, false);

        if (apply != select_no_match) {
            used_selector::ptr us =
                std::unique_ptr<used_selector>(new used_selector(sel, false));

            if (sel->is_media_valid()) {
                if (apply & select_match_pseudo_class) {
                    if (select(*sel, true)) {
                        if (apply & select_match_with_after) {
                            Element::ptr el = get_element_after();
                            if (el) {
                                el->add_style(*sel->m_style);
                            }
                        } else if (apply & select_match_with_before) {
                            Element::ptr el = get_element_before();
                            if (el) {
                                el->add_style(*sel->m_style);
                            }
                        } else {
                            add_style(*sel->m_style);
                            us->m_used = true;
                        }
                    }
                } else if (apply & select_match_with_after) {
                    Element::ptr el = get_element_after();
                    if (el) {
                        el->add_style(*sel->m_style);
                    }
                } else if (apply & select_match_with_before) {
                    Element::ptr el = get_element_before();
                    if (el) {
                        el->add_style(*sel->m_style);
                    }
                } else {
                    add_style(*sel->m_style);
                    us->m_used = true;
                }
            }
            m_used_styles.push_back(std::move(us));
        }
    }

    for (auto& el : m_children) {
        if (el->get_display() != kDisplayInlineText) {
            el->apply_stylesheet(stylesheet);
        }
    }
}

void HTMLElement::get_content_size(Size& sz, int max_width)
{
    sz.height = 0;
    if (m_display == kDisplayBlock) {
        sz.width = max_width;
    } else {
        sz.width = 0;
    }
}

void HTMLElement::draw(uintptr_t hdc, int x, int y, const Position* clip)
{
    Position pos = position_;
    pos.x += x;
    pos.y += y;

    draw_background(hdc, x, y, clip);

    if (m_display == kDisplayListItem &&
        list_style_type_ != kListStyleTypeNone) {
        if (overflow_ > kOverflowVisible) {
            Position border_box = pos;
            border_box += m_padding;
            border_box += m_borders;

            BorderRadii border_radii = m_css_borders.radii.calculate_radii(border_box.width, border_box.height);

            border_radii -= m_borders;
            border_radii -= m_padding;

            get_document()->container()->set_clip(pos, border_radii, true, true);
        }

        draw_list_marker(hdc, pos);

        if (overflow_ > kOverflowVisible) {
            get_document()->container()->del_clip();
        }
    }
}

uintptr_t HTMLElement::get_font(FontMetrics* fm)
{
    if (fm) {
        *fm = font_metrics_;
    }
    return font_;
}

const tchar_t* HTMLElement::get_style_property(CSSProperty name)
{
    const tchar_t* value = m_style.get_property(name);

    if (parent()) {
        if (value && !t_strcasecmp(value, _t("inherit"))) {
            value = parent()->get_style_property(name);
        } else if (!value && css_property_inherited(name)) {
            value = parent()->get_style_property(name);
        }
    }

    if (!value) {
        value = css_property_default(name);
    }

    return value;
}

const CSSValue* HTMLElement::get_style_property_value(CSSProperty property) const
{
    const CSSValue* value = m_style.get_property_value(property);

    if (parent()) {
        if (value && value->inherit()) {
            value = parent()->get_style_property_value(property);
        } else if (!value && css_property_inherited(property)) {
            value = parent()->get_style_property_value(property);
        }
    }

    if (!value) {
        value = css_property_default_value(property);
    }

    return value;
}

void HTMLElement::parse_styles(bool is_reparse)
{
    const tchar_t* style = get_attr(_t("style"));

    if (style) {
        m_style.add(style, URL());
    }

    init_font();
    Document* doc = get_document();

    m_el_position = get_keyword<ElementPosition>(kCSSPropertyPosition);

    m_text_align = get_keyword<TextAlign>(kCSSPropertyTextAlign);

    overflow_ = get_keyword<Overflow>(kCSSPropertyOverflow);

    white_space_ = get_keyword<WhiteSpace>(kCSSPropertyWhiteSpace);

    m_display = get_keyword<Display>(kCSSPropertyDisplay);

    m_visibility = get_keyword<Visibility>(kCSSPropertyVisibility);

    box_sizing_ = get_keyword<BoxSizing>(kCSSPropertyBoxSizing);

    if (m_el_position != kPositionStatic) {
        const tchar_t* val = get_style_property(kCSSPropertyZIndex);
        if (val) {
            m_z_index = t_atoi(val);
        }
    }

    vertical_align_ = get_keyword<VerticalAlign>(kCSSPropertyVerticalAlign);

    m_float = get_keyword<ElementFloat>(kCSSPropertyFloat);

    m_clear = get_keyword<ElementClear>(kCSSPropertyClear);

    if (m_float != kFloatNone) {
        // reset display in to block for floating elements
        if (m_display != kDisplayNone) {
            m_display = kDisplayBlock;
        }
    } else if (m_display == kDisplayTable || m_display == kDisplayInlineTable ||
               m_display == kDisplayTableCaption ||
               m_display == kDisplayTableCell ||
               m_display == kDisplayTableColumn ||
               m_display == kDisplayTableColumnGroup ||
               m_display == kDisplayTableFooterGroup ||
               m_display == kDisplayTableHeaderGroup ||
               m_display == kDisplayTableRow ||
               m_display == kDisplayTableRowGroup) {
        doc->add_tabular(this);
    }
    // fix inline boxes with absolute/fixed positions
    else if (m_display != kDisplayNone && is_inline_box()) {
        if (m_el_position == kPositionAbsolute ||
            m_el_position == kPositionFixed) {
            m_display = kDisplayBlock;
        }
    }

    m_css_text_indent = get_length(kCSSPropertyTextIndent);

    m_css_width = get_length(kCSSPropertyWidth);
    m_css_height = get_length(kCSSPropertyHeight);

    doc->cvt_units(m_css_width, font_size_);
    doc->cvt_units(m_css_height, font_size_);

    m_css_min_width = get_length(kCSSPropertyMinWidth);
    m_css_min_height = get_length(kCSSPropertyMinHeight);

    m_css_max_width = get_length(kCSSPropertyMaxWidth);
    m_css_max_height = get_length(kCSSPropertyMaxHeight);

    doc->cvt_units(m_css_min_width, font_size_);
    doc->cvt_units(m_css_min_height, font_size_);

    m_css_offsets.left = get_length(kCSSPropertyLeft);
    m_css_offsets.right = get_length(kCSSPropertyRight);
    m_css_offsets.top = get_length(kCSSPropertyTop);
    m_css_offsets.bottom = get_length(kCSSPropertyBottom);

    doc->cvt_units(m_css_offsets.left, font_size_);
    doc->cvt_units(m_css_offsets.right, font_size_);
    doc->cvt_units(m_css_offsets.top, font_size_);
    doc->cvt_units(m_css_offsets.bottom, font_size_);

    m_css_margins.left = get_length(kCSSPropertyMarginLeft);
    m_css_margins.right = get_length(kCSSPropertyMarginRight);
    m_css_margins.top = get_length(kCSSPropertyMarginTop);
    m_css_margins.bottom = get_length(kCSSPropertyMarginBottom);

    m_css_padding.left = get_length(kCSSPropertyPaddingLeft);
    m_css_padding.right = get_length(kCSSPropertyPaddingRight);
    m_css_padding.top = get_length(kCSSPropertyPaddingTop);
    m_css_padding.bottom = get_length(kCSSPropertyPaddingBottom);

    m_css_borders.left.width = get_length(kCSSPropertyBorderLeftWidth);
    m_css_borders.right.width = get_length(kCSSPropertyBorderRightWidth);
    m_css_borders.top.width = get_length(kCSSPropertyBorderTopWidth);
    m_css_borders.bottom.width = get_length(kCSSPropertyBorderBottomWidth);

    m_css_borders.left.color = get_color(kCSSPropertyBorderLeftColor);
    m_css_borders.left.style = get_keyword<BorderStyle>(kCSSPropertyBorderLeftStyle);

    m_css_borders.right.color = get_color(kCSSPropertyBorderRightColor);
    m_css_borders.right.style = get_keyword<BorderStyle>(kCSSPropertyBorderRightStyle);

    m_css_borders.top.color = get_color(kCSSPropertyBorderTopColor);
    m_css_borders.top.style = get_keyword<BorderStyle>(kCSSPropertyBorderTopStyle);

    m_css_borders.bottom.color = get_color(kCSSPropertyBorderBottomColor);
    m_css_borders.bottom.style = get_keyword<BorderStyle>(kCSSPropertyBorderBottomStyle);

    m_css_borders.radii.top_left.x = get_length(kCSSPropertyBorderTopLeftRadiusX);
    m_css_borders.radii.top_left.y = get_length(kCSSPropertyBorderTopLeftRadiusY);
    m_css_borders.radii.top_right.x = get_length(kCSSPropertyBorderTopRightRadiusX);
    m_css_borders.radii.top_right.y = get_length(kCSSPropertyBorderTopRightRadiusY);
    m_css_borders.radii.bottom_right.x = get_length(kCSSPropertyBorderBottomRightRadiusX);
    m_css_borders.radii.bottom_right.y = get_length(kCSSPropertyBorderBottomRightRadiusY);
    m_css_borders.radii.bottom_left.x = get_length(kCSSPropertyBorderBottomLeftRadiusX);
    m_css_borders.radii.bottom_left.y = get_length(kCSSPropertyBorderBottomLeftRadiusY);

    doc->cvt_units(m_css_borders.radii.bottom_left.x, font_size_);
    doc->cvt_units(m_css_borders.radii.bottom_left.y, font_size_);
    doc->cvt_units(m_css_borders.radii.bottom_right.x, font_size_);
    doc->cvt_units(m_css_borders.radii.bottom_right.y, font_size_);
    doc->cvt_units(m_css_borders.radii.top_left.x, font_size_);
    doc->cvt_units(m_css_borders.radii.top_left.y, font_size_);
    doc->cvt_units(m_css_borders.radii.top_right.x, font_size_);
    doc->cvt_units(m_css_borders.radii.top_right.y, font_size_);

    doc->cvt_units(m_css_text_indent, font_size_);

    m_margins.left = doc->cvt_units(m_css_margins.left, font_size_);
    m_margins.right = doc->cvt_units(m_css_margins.right, font_size_);
    m_margins.top = doc->cvt_units(m_css_margins.top, font_size_);
    m_margins.bottom = doc->cvt_units(m_css_margins.bottom, font_size_);

    m_padding.left = doc->cvt_units(m_css_padding.left, font_size_);
    m_padding.right = doc->cvt_units(m_css_padding.right, font_size_);
    m_padding.top = doc->cvt_units(m_css_padding.top, font_size_);
    m_padding.bottom = doc->cvt_units(m_css_padding.bottom, font_size_);

    m_borders.left = doc->cvt_units(m_css_borders.left.width, font_size_);
    m_borders.right = doc->cvt_units(m_css_borders.right.width, font_size_);
    m_borders.top = doc->cvt_units(m_css_borders.top.width, font_size_);
    m_borders.bottom = doc->cvt_units(m_css_borders.bottom.width, font_size_);

    CSSLength line_height = get_length(kCSSPropertyLineHeight);

    // FIXME: Inspect the line height keyword to properly calculate the line
    // height.
    if (line_height.is_predefined()) {
        line_height_ = font_metrics_.height;
        m_lh_predefined = true;
    } else if (line_height.units() == kCSSUnitsNone) {
        line_height_ = (int)(line_height.val() * font_size_);
        m_lh_predefined = false;
    } else {
        line_height_ = doc->cvt_units(line_height, font_size_, font_size_);
        m_lh_predefined = false;
    }


    if (m_display == kDisplayListItem) {
        list_style_type_ = get_keyword<ListStyleType>(kCSSPropertyListStyleType);

        list_style_position_ = get_keyword<ListStylePosition>(kCSSPropertyListStylePosition);

        const tchar_t* list_image = get_style_property(kCSSPropertyListStyleImage);
        if (list_image && list_image[0] && t_strcasecmp(list_image, "none")) {
            tstring url;
            CSSStylesheet::parse_css_url(list_image, url);

            // const tchar_t* list_image_baseurl = get_style_property(
            //     kCSSPropertyListStyleImageBaseurl,
            //     true,
            //     nullptr);
            //
            // assert(list_image_baseurl != nullptr);
            assert(false);
            // doc->container()->load_image(url.c_str(), list_image_baseurl, true);
        }
    }

    parse_background();

    if (!is_reparse) {
        for (auto& el : m_children) {
            el->parse_styles();
        }
    }
}

int HTMLElement::render(int x, int y, int max_width, bool second_pass)
{
    if (m_display == kDisplayTable || m_display == kDisplayInlineTable) {
        return render_table(x, y, max_width, second_pass);
    }

    return render_box(x, y, max_width, second_pass);
}

bool HTMLElement::is_white_space() const
{
    return false;
}

int HTMLElement::get_font_size() const
{
    return font_size_;
}

int HTMLElement::get_baseline()
{
    if (is_replaced()) {
        return 0;
    }
    int bl = 0;
    if (!m_boxes.empty()) {
        bl = m_boxes.back()->baseline() + content_margins_bottom();
    }
    return bl;
}

void HTMLElement::init()
{
    if (m_display == kDisplayTable || m_display == kDisplayInlineTable) {
        if (m_grid) {
            m_grid->clear();
        } else {
            m_grid = std::unique_ptr<table_grid>(new table_grid());
        }

        go_inside_table table_selector;
        table_rows_selector row_selector;
        table_cells_selector cell_selector;

        elements_iterator row_iter(this, &table_selector, &row_selector);

        Element::ptr row = row_iter.next(false);
        while (row) {
            m_grid->begin_row(row);

            elements_iterator cell_iter(row, &table_selector, &cell_selector);
            Element::ptr cell = cell_iter.next();
            while (cell) {
                m_grid->add_cell(cell);

                cell = cell_iter.next(false);
            }
            row = row_iter.next(false);
        }

        m_grid->finish();
    }

    for (auto& el : m_children) {
        el->init();
    }
}

int HTMLElement::select(const CSSSelector& selector, bool apply_pseudo)
{
    int right_res = select(selector.m_right, apply_pseudo);
    if (right_res == select_no_match) {
        return select_no_match;
    }
    Element::ptr el_parent = parent();
    if (selector.m_left) {
        if (!el_parent) {
            return select_no_match;
        }
        switch (selector.m_combinator) {
            case combinator_descendant: {
                bool is_pseudo = false;
                Element::ptr res =
                    find_ancestor(*selector.m_left, apply_pseudo, &is_pseudo);
                if (!res) {
                    return select_no_match;
                } else {
                    if (is_pseudo) {
                        right_res |= select_match_pseudo_class;
                    }
                }
            } break;
            case combinator_child: {
                int res = el_parent->select(*selector.m_left, apply_pseudo);
                if (res == select_no_match) {
                    return select_no_match;
                } else {
                    if (right_res != select_match_pseudo_class) {
                        right_res |= res;
                    }
                }
            } break;
            case combinator_adjacent_sibling: {
                bool is_pseudo = false;
                Element::ptr res =
                    el_parent->find_adjacent_sibling(this,
                        *selector.m_left,
                        apply_pseudo,
                        &is_pseudo);
                if (!res) {
                    return select_no_match;
                } else {
                    if (is_pseudo) {
                        right_res |= select_match_pseudo_class;
                    }
                }
            } break;
            case combinator_general_sibling: {
                bool is_pseudo = false;
                Element::ptr res = el_parent->find_sibling(this,
                    *selector.m_left,
                    apply_pseudo,
                    &is_pseudo);
                if (!res) {
                    return select_no_match;
                } else {
                    if (is_pseudo) {
                        right_res |= select_match_pseudo_class;
                    }
                }
            } break;
            default:
                right_res = select_no_match;
        }
    }
    return right_res;
}

int HTMLElement::select(const css_element_selector& selector, bool apply_pseudo)
{
    if (!selector.m_tag.empty() && selector.m_tag != _t("*")) {
        if (selector.m_tag != m_tag) {
            return select_no_match;
        }
    }

    int res = select_match;
    Element::ptr el_parent = parent();

    for (css_attribute_selector::vector::const_iterator i =
             selector.m_attrs.begin();
         i != selector.m_attrs.end();
         i++) {
        const tchar_t* attr_value = get_attr(i->attribute.c_str());
        switch (i->condition) {
            case select_exists:
                if (!attr_value) {
                    return select_no_match;
                }
                break;
            case select_equal:
                if (!attr_value) {
                    return select_no_match;
                } else {
                    if (i->attribute == _t("class")) {
                        const string_vector& tokens1 = m_class_values;
                        const string_vector& tokens2 = i->class_val;
                        bool found = true;
                        for (string_vector::const_iterator str1 = tokens2.begin();
                             str1 != tokens2.end() && found;
                             str1++) {
                            bool f = false;
                            for (string_vector::const_iterator str2 =
                                     tokens1.begin();
                                 str2 != tokens1.end() && !f;
                                 str2++) {
                                if (!t_strcasecmp(str1->c_str(), str2->c_str())) {
                                    f = true;
                                }
                            }
                            if (!f) {
                                found = false;
                            }
                        }
                        if (!found) {
                            return select_no_match;
                        }
                    } else {
                        if (t_strcasecmp(i->val.c_str(), attr_value)) {
                            return select_no_match;
                        }
                    }
                }
                break;
            case select_contain_str:
                if (!attr_value) {
                    return select_no_match;
                } else if (!t_strstr(attr_value, i->val.c_str())) {
                    return select_no_match;
                }
                break;
            case select_start_str:
                if (!attr_value) {
                    return select_no_match;
                } else if (t_strncmp(attr_value, i->val.c_str(), i->val.length())) {
                    return select_no_match;
                }
                break;
            case select_end_str:
                if (!attr_value) {
                    return select_no_match;
                } else if (t_strncmp(attr_value, i->val.c_str(), i->val.length())) {
                    const tchar_t* s =
                        attr_value + t_strlen(attr_value) - i->val.length() - 1;
                    if (s < attr_value) {
                        return select_no_match;
                    }
                    if (i->val != s) {
                        return select_no_match;
                    }
                }
                break;
            case select_pseudo_element:
                if (i->val == _t("after")) {
                    res |= select_match_with_after;
                } else if (i->val == _t("before")) {
                    res |= select_match_with_before;
                } else {
                    return select_no_match;
                }
                break;
            case select_pseudo_class:
                if (apply_pseudo) {
                    if (!el_parent)
                        return select_no_match;

                    tstring selector_param;
                    tstring selector_name;

                    tstring::size_type begin = i->val.find_first_of(_t('('));
                    tstring::size_type end =
                        (begin == tstring::npos)
                            ? tstring::npos
                            : find_close_bracket(i->val, begin);
                    if (begin != tstring::npos && end != tstring::npos) {
                        selector_param = i->val.substr(begin + 1, end - begin - 1);
                    }
                    if (begin != tstring::npos) {
                        selector_name = i->val.substr(0, begin);
                        trim(selector_name);
                    } else {
                        selector_name = i->val;
                    }

                    int other_selector =
                        value_index(selector_name.c_str(), PSEUDO_CLASS_STRINGS);

                    switch (other_selector) {
                        case pseudo_class_only_child:
                            if (!el_parent->is_only_child(this,
                                    false)) {
                                return select_no_match;
                            }
                            break;
                        case pseudo_class_only_of_type:
                            if (!el_parent->is_only_child(this, true)) {
                                return select_no_match;
                            }
                            break;
                        case pseudo_class_first_child:
                            if (!el_parent->is_nth_child(this,
                                    0,
                                    1,
                                    false)) {
                                return select_no_match;
                            }
                            break;
                        case pseudo_class_first_of_type:
                            if (!el_parent->is_nth_child(this, 0, 1, true)) {
                                return select_no_match;
                            }
                            break;
                        case pseudo_class_last_child:
                            if (!el_parent->is_nth_last_child(this,
                                    0,
                                    1,
                                    false)) {
                                return select_no_match;
                            }
                            break;
                        case pseudo_class_last_of_type:
                            if (!el_parent->is_nth_last_child(this,
                                    0,
                                    1,
                                    true)) {
                                return select_no_match;
                            }
                            break;
                        case pseudo_class_nth_child:
                        case pseudo_class_nth_of_type:
                        case pseudo_class_nth_last_child:
                        case pseudo_class_nth_last_of_type: {
                            if (selector_param.empty())
                                return select_no_match;

                            int num = 0;
                            int off = 0;

                            parse_nth_child_params(selector_param, num, off);
                            if (!num && !off)
                                return select_no_match;
                            switch (other_selector) {
                                case pseudo_class_nth_child:
                                    if (!el_parent->is_nth_child(this,
                                            num,
                                            off,
                                            false)) {
                                        return select_no_match;
                                    }
                                    break;
                                case pseudo_class_nth_of_type:
                                    if (!el_parent->is_nth_child(this,
                                            num,
                                            off,
                                            true)) {
                                        return select_no_match;
                                    }
                                    break;
                                case pseudo_class_nth_last_child:
                                    if (!el_parent->is_nth_last_child(
                                            this,
                                            num,
                                            off,
                                            false)) {
                                        return select_no_match;
                                    }
                                    break;
                                case pseudo_class_nth_last_of_type:
                                    if (!el_parent->is_nth_last_child(
                                            this,
                                            num,
                                            off,
                                            true)) {
                                        return select_no_match;
                                    }
                                    break;
                            }

                        } break;
                        case pseudo_class_not: {
                            css_element_selector sel;
                            sel.parse(selector_param);
                            if (select(sel, apply_pseudo)) {
                                return select_no_match;
                            }
                        } break;
                        case pseudo_class_lang: {
                            trim(selector_param);

                            if (!get_document()->match_lang(selector_param)) {
                                return select_no_match;
                            }
                        } break;
                        default:
                            if (std::find(m_pseudo_classes.begin(),
                                    m_pseudo_classes.end(),
                                    i->val) == m_pseudo_classes.end()) {
                                return select_no_match;
                            }
                            break;
                    }
                } else {
                    res |= select_match_pseudo_class;
                }
                break;
        }
    }
    return res;
}

Element::ptr HTMLElement::find_ancestor(const CSSSelector& selector,
    bool apply_pseudo,
    bool* is_pseudo)
{
    Element::ptr el_parent = parent();
    if (!el_parent) {
        return nullptr;
    }
    int res = el_parent->select(selector, apply_pseudo);
    if (res != select_no_match) {
        if (is_pseudo) {
            if (res & select_match_pseudo_class) {
                *is_pseudo = true;
            } else {
                *is_pseudo = false;
            }
        }
        return el_parent;
    }
    return el_parent->find_ancestor(selector, apply_pseudo, is_pseudo);
}

int HTMLElement::get_floats_height(ElementFloat el_float) const
{
    if (is_floats_holder()) {
        int h = 0;

        bool process = false;

        for (const auto& fb : m_floats_left) {
            process = false;
            switch (el_float) {
                case kFloatNone:
                    process = true;
                    break;
                case kFloatLeft:
                    if (fb.clear_floats == kClearLeft ||
                        fb.clear_floats == kClearBoth) {
                        process = true;
                    }
                    break;
                case kFloatRight:
                    if (fb.clear_floats == kClearRight ||
                        fb.clear_floats == kClearBoth) {
                        process = true;
                    }
                    break;
            }
            if (process) {
                if (el_float == kFloatNone) {
                    h = std::max(h, fb.pos.bottom());
                } else {
                    h = std::max(h, fb.pos.top());
                }
            }
        }


        for (const auto& fb : m_floats_right) {
            process = false;
            switch (el_float) {
                case kFloatNone:
                    process = true;
                    break;
                case kFloatLeft:
                    if (fb.clear_floats == kClearLeft ||
                        fb.clear_floats == kClearBoth) {
                        process = true;
                    }
                    break;
                case kFloatRight:
                    if (fb.clear_floats == kClearRight ||
                        fb.clear_floats == kClearBoth) {
                        process = true;
                    }
                    break;
            }
            if (process) {
                if (el_float == kFloatNone) {
                    h = std::max(h, fb.pos.bottom());
                } else {
                    h = std::max(h, fb.pos.top());
                }
            }
        }

        return h;
    }
    Element::ptr el_parent = parent();
    if (el_parent) {
        int h = el_parent->get_floats_height(el_float);
        return h - position_.y;
    }
    return 0;
}

int HTMLElement::get_left_floats_height() const
{
    if (is_floats_holder()) {
        int h = 0;
        if (!m_floats_left.empty()) {
            for (const auto& fb : m_floats_left) {
                h = std::max(h, fb.pos.bottom());
            }
        }
        return h;
    }
    Element::ptr el_parent = parent();
    if (el_parent) {
        int h = el_parent->get_left_floats_height();
        return h - position_.y;
    }
    return 0;
}

int HTMLElement::get_right_floats_height() const
{
    if (is_floats_holder()) {
        int h = 0;
        if (!m_floats_right.empty()) {
            for (const auto& fb : m_floats_right) {
                h = std::max(h, fb.pos.bottom());
            }
        }
        return h;
    }
    Element::ptr el_parent = parent();
    if (el_parent) {
        int h = el_parent->get_right_floats_height();
        return h - position_.y;
    }
    return 0;
}

int HTMLElement::get_line_left(int y)
{
    if (is_floats_holder()) {
        if (m_cahe_line_left.is_valid && m_cahe_line_left.hash == y) {
            return m_cahe_line_left.val;
        }

        int w = 0;
        for (const auto& fb : m_floats_left) {
            if (y >= fb.pos.top() && y < fb.pos.bottom()) {
                w = std::max(w, fb.pos.right());
                if (w < fb.pos.right()) {
                    break;
                }
            }
        }
        m_cahe_line_left.set_value(y, w);
        return w;
    }
    Element::ptr el_parent = parent();
    if (el_parent) {
        int w = el_parent->get_line_left(y + position_.y);
        if (w < 0) {
            w = 0;
        }
        return w - (w ? position_.x : 0);
    }
    return 0;
}

int HTMLElement::get_line_right(int y, int def_right)
{
    if (is_floats_holder()) {
        if (m_cahe_line_right.is_valid && m_cahe_line_right.hash == y) {
            if (m_cahe_line_right.is_default) {
                return def_right;
            } else {
                return std::min(m_cahe_line_right.val, def_right);
            }
        }

        int w = def_right;
        m_cahe_line_right.is_default = true;
        for (const auto& fb : m_floats_right) {
            if (y >= fb.pos.top() && y < fb.pos.bottom()) {
                w = std::min(w, fb.pos.left());
                m_cahe_line_right.is_default = false;
                if (w > fb.pos.left()) {
                    break;
                }
            }
        }
        m_cahe_line_right.set_value(y, w);
        return w;
    }
    Element::ptr el_parent = parent();
    if (el_parent) {
        int w = el_parent->get_line_right(y + position_.y, def_right + position_.x);
        return w - position_.x;
    }
    return 0;
}


void HTMLElement::get_line_left_right(int y, int def_right, int& ln_left, int& ln_right)
{
    if (is_floats_holder()) {
        ln_left = get_line_left(y);
        ln_right = get_line_right(y, def_right);
    } else {
        Element::ptr el_parent = parent();
        if (el_parent) {
            el_parent->get_line_left_right(y + position_.y,
                def_right + position_.x,
                ln_left,
                ln_right);
        }
        ln_right -= position_.x;

        if (ln_left < 0) {
            ln_left = 0;
        } else if (ln_left > 0) {
            ln_left -= position_.x;
            if (ln_left < 0) {
                ln_left = 0;
            }
        }
    }
}

int HTMLElement::fix_line_width(int max_width, ElementFloat flt)
{
    int ret_width = 0;
    if (!m_boxes.empty()) {
        ElementsVector els;
        m_boxes.back()->get_elements(els);
        bool was_cleared = false;
        if (!els.empty() && els.front()->get_clear() != kClearNone) {
            if (els.front()->get_clear() == kClearBoth) {
                was_cleared = true;
            } else {
                if ((flt == kFloatLeft && els.front()->get_clear() == kClearLeft) ||
                    (flt == kFloatRight && els.front()->get_clear() == kClearRight)) {
                    was_cleared = true;
                }
            }
        }

        if (!was_cleared) {
            m_boxes.pop_back();

            for (ElementsVector::iterator i = els.begin(); i != els.end(); i++) {
                int rw = place_element((*i), max_width);
                if (rw > ret_width) {
                    ret_width = rw;
                }
            }
        } else {
            int line_top = 0;
            if (m_boxes.back()->get_type() == kBoxLine) {
                line_top = m_boxes.back()->top();
            } else {
                line_top = m_boxes.back()->bottom();
            }

            int line_left = 0;
            int line_right = max_width;
            get_line_left_right(line_top, max_width, line_left, line_right);

            if (m_boxes.back()->get_type() == kBoxLine) {
                if (m_boxes.size() == 1 &&
                    list_style_type_ != kListStyleTypeNone &&
                    list_style_position_ == kListStylePositionInside) {
                    int sz_font = get_font_size();
                    line_left += sz_font;
                }

                if (m_css_text_indent.val() != 0.0f) {
                    bool line_box_found = false;
                    for (Box::vector::iterator iter = m_boxes.begin();
                         iter < m_boxes.end();
                         iter++) {
                        if ((*iter)->get_type() == kBoxLine) {
                            line_box_found = true;
                            break;
                        }
                    }
                    if (!line_box_found) {
                        line_left += m_css_text_indent.calc_percent(max_width);
                    }
                }
            }

            ElementsVector elements;
            m_boxes.back()->new_width(line_left, line_right, elements);
            for (auto& el : elements) {
                int rw = place_element(el, max_width);
                if (rw > ret_width) {
                    ret_width = rw;
                }
            }
        }
    }

    return ret_width;
}

void HTMLElement::add_float(const Element::ptr& el, int x, int y)
{
    if (is_floats_holder()) {
        floated_box fb;
        fb.pos.x = el->left() + x;
        fb.pos.y = el->top() + y;
        fb.pos.width = el->width();
        fb.pos.height = el->height();
        fb.float_side = el->get_float();
        fb.clear_floats = el->get_clear();
        fb.el = el;

        if (fb.float_side == kFloatLeft) {
            if (m_floats_left.empty()) {
                m_floats_left.push_back(fb);
            } else {
                bool inserted = false;
                for (floated_box::vector::iterator i = m_floats_left.begin();
                     i != m_floats_left.end();
                     i++) {
                    if (fb.pos.right() > i->pos.right()) {
                        m_floats_left.insert(i, std::move(fb));
                        inserted = true;
                        break;
                    }
                }
                if (!inserted) {
                    m_floats_left.push_back(std::move(fb));
                }
            }
            m_cahe_line_left.invalidate();
        } else if (fb.float_side == kFloatRight) {
            if (m_floats_right.empty()) {
                m_floats_right.push_back(std::move(fb));
            } else {
                bool inserted = false;
                for (floated_box::vector::iterator i = m_floats_right.begin();
                     i != m_floats_right.end();
                     i++) {
                    if (fb.pos.left() < i->pos.left()) {
                        m_floats_right.insert(i, std::move(fb));
                        inserted = true;
                        break;
                    }
                }
                if (!inserted) {
                    m_floats_right.push_back(fb);
                }
            }
            m_cahe_line_right.invalidate();
        }
    } else {
        Element::ptr el_parent = parent();
        if (el_parent) {
            el_parent->add_float(el, x + position_.x, y + position_.y);
        }
    }
}

int HTMLElement::find_next_line_top(int top, int width, int def_right)
{
    if (is_floats_holder()) {
        int new_top = top;
        int_vector points;

        for (const auto& fb : m_floats_left) {
            if (fb.pos.top() >= top) {
                if (find(points.begin(), points.end(), fb.pos.top()) ==
                    points.end()) {
                    points.push_back(fb.pos.top());
                }
            }
            if (fb.pos.bottom() >= top) {
                if (find(points.begin(), points.end(), fb.pos.bottom()) ==
                    points.end()) {
                    points.push_back(fb.pos.bottom());
                }
            }
        }

        for (const auto& fb : m_floats_right) {
            if (fb.pos.top() >= top) {
                if (find(points.begin(), points.end(), fb.pos.top()) ==
                    points.end()) {
                    points.push_back(fb.pos.top());
                }
            }
            if (fb.pos.bottom() >= top) {
                if (find(points.begin(), points.end(), fb.pos.bottom()) ==
                    points.end()) {
                    points.push_back(fb.pos.bottom());
                }
            }
        }

        if (!points.empty()) {
            sort(points.begin(), points.end(), std::less<int>());
            new_top = points.back();

            for (auto pt : points) {
                int pos_left = 0;
                int pos_right = def_right;
                get_line_left_right(pt, def_right, pos_left, pos_right);

                if (pos_right - pos_left >= width) {
                    new_top = pt;
                    break;
                }
            }
        }
        return new_top;
    }
    Element::ptr el_parent = parent();
    if (el_parent) {
        int new_top = el_parent->find_next_line_top(top + position_.y,
            width,
            def_right + position_.x);
        return new_top - position_.y;
    }
    return 0;
}

void HTMLElement::parse_background()
{
    m_bg.m_color = get_color(kCSSPropertyBackgroundColor, Color(0, 0, 0, 0));

    const tchar_t* str = get_style_property(kCSSPropertyBackgroundPosition);
    if (str) {
        string_vector res;
        split_string(str, res, _t(" \t"));
        if (res.size() > 0) {
            if (res.size() == 1) {
                if (value_in_list(res[0].c_str(), _t("left;right;center"))) {
                    m_bg.m_position.x.parse_length_string(res[0], _t("left;right;center"));
                    m_bg.m_position.y.set_value(50, kCSSUnitsPercent);
                } else if (value_in_list(res[0].c_str(), _t("top;bottom;center"))) {
                    m_bg.m_position.y.parse_length_string(res[0], _t("top;bottom;center"));
                    m_bg.m_position.x.set_value(50, kCSSUnitsPercent);
                } else {
                    m_bg.m_position.x.parse_length_string(res[0], _t("left;right;center"));
                    m_bg.m_position.y.set_value(50, kCSSUnitsPercent);
                }
            } else {
                if (value_in_list(res[0].c_str(), _t("left;right"))) {
                    m_bg.m_position.x.parse_length_string(res[0], _t("left;right;center"));
                    m_bg.m_position.y.parse_length_string(res[1], _t("top;bottom;center"));
                } else if (value_in_list(res[0].c_str(), _t("top;bottom"))) {
                    m_bg.m_position.x.parse_length_string(res[1], _t("left;right;center"));
                    m_bg.m_position.y.parse_length_string(res[0], _t("top;bottom;center"));
                } else if (value_in_list(res[1].c_str(), _t("left;right"))) {
                    m_bg.m_position.x.parse_length_string(res[1], _t("left;right;center"));
                    m_bg.m_position.y.parse_length_string(res[0], _t("top;bottom;center"));
                } else if (value_in_list(res[1].c_str(), _t("top;bottom"))) {
                    m_bg.m_position.x.parse_length_string(res[0], _t("left;right;center"));
                    m_bg.m_position.y.parse_length_string(res[1], _t("top;bottom;center"));
                } else {
                    m_bg.m_position.x.parse_length_string(res[0], _t("left;right;center"));
                    m_bg.m_position.y.parse_length_string(res[1], _t("top;bottom;center"));
                }
            }

            if (m_bg.m_position.x.is_predefined()) {
                switch (m_bg.m_position.x.predef()) {
                    case 0:
                        m_bg.m_position.x.set_value(0, kCSSUnitsPercent);
                        break;
                    case 1:
                        m_bg.m_position.x.set_value(100, kCSSUnitsPercent);
                        break;
                    case 2:
                        m_bg.m_position.x.set_value(50, kCSSUnitsPercent);
                        break;
                }
            }
            if (m_bg.m_position.y.is_predefined()) {
                switch (m_bg.m_position.y.predef()) {
                    case 0:
                        m_bg.m_position.y.set_value(0, kCSSUnitsPercent);
                        break;
                    case 1:
                        m_bg.m_position.y.set_value(100, kCSSUnitsPercent);
                        break;
                    case 2:
                        m_bg.m_position.y.set_value(50, kCSSUnitsPercent);
                        break;
                }
            }
        } else {
            m_bg.m_position.x.set_value(0, kCSSUnitsPercent);
            m_bg.m_position.y.set_value(0, kCSSUnitsPercent);
        }
    } else {
        m_bg.m_position.y.set_value(0, kCSSUnitsPercent);
        m_bg.m_position.x.set_value(0, kCSSUnitsPercent);
    }

    str = get_style_property(kCSSPropertyBackgroundSize);
    if (str) {
        string_vector res;
        split_string(str, res, _t(" \t"));
        if (!res.empty()) {
            m_bg.m_position.width.parse_length_string(res[0], BACKGROUND_SIZE_STRINGS);
            if (res.size() > 1) {
                m_bg.m_position.height.parse_length_string(res[1], BACKGROUND_SIZE_STRINGS);
            } else {
                m_bg.m_position.height.predef(background_size_auto);
            }
        } else {
            m_bg.m_position.width.predef(background_size_auto);
            m_bg.m_position.height.predef(background_size_auto);
        }
    }

    Document* doc = get_document();

    doc->cvt_units(m_bg.m_position.x, font_size_);
    doc->cvt_units(m_bg.m_position.y, font_size_);
    doc->cvt_units(m_bg.m_position.width, font_size_);
    doc->cvt_units(m_bg.m_position.height, font_size_);

    // parse background-attachment
    m_bg.m_attachment = get_keyword<BackgroundAttachment>(kCSSPropertyBackgroundAttachment);

    // parse background-repeat
    m_bg.m_repeat = get_keyword<BackgroundRepeat>(kCSSPropertyBackgroundRepeat);

    // parse background-clip
    m_bg.m_clip = get_keyword<BackgroundBox>(kCSSPropertyBackgroundClip);

    // parse background-origin
    m_bg.m_origin = get_keyword<BackgroundBox>(kCSSPropertyBackgroundOrigin);

    // parse background-image

    // kCSSPropertyBackgroundImageBaseurl is an internal litehtml property.
    // It's not clear how it's used but we should probably just ignore it
    // and resolve URLs using the document base URL.

    // Check that the kCSSPropertyBackgroundImageBaseurl property is empty.
    // If it's not empty we should track down what is setting the property.
    tstring baseurl = get_style_property(kCSSPropertyBackgroundImageBaseurl);
    assert(baseurl.empty());

    tstring url;
    CSSStylesheet::parse_css_url(
        get_style_property(kCSSPropertyBackgroundImage),
        url);

    if (!url.empty()) {
        m_bg.m_image = resolve(doc->base_url(), URL(url));
        doc->container()->load_image(m_bg.m_image, true);
    }
}

void HTMLElement::add_positioned(const Element::ptr& el)
{
    if (m_el_position != kPositionStatic || (!have_parent())) {
        m_positioned.push_back(el);
    } else {
        Element::ptr el_parent = parent();
        if (el_parent) {
            el_parent->add_positioned(el);
        }
    }
}

void HTMLElement::calc_outlines(int parent_width)
{
    m_margins.left = m_css_margins.left.calc_percent(parent_width);
    m_margins.right = m_css_margins.right.calc_percent(parent_width);
    m_margins.top = m_css_margins.top.calc_percent(parent_width);
    m_margins.bottom = m_css_margins.bottom.calc_percent(parent_width);

    m_padding.left = m_css_padding.left.calc_percent(parent_width);
    m_padding.right = m_css_padding.right.calc_percent(parent_width);
    m_padding.top = m_css_padding.top.calc_percent(parent_width);
    m_padding.bottom = m_css_padding.bottom.calc_percent(parent_width);

    m_borders.left = m_css_borders.left.width.calc_percent(parent_width);
    m_borders.right = m_css_borders.right.width.calc_percent(parent_width);
    m_borders.top = m_css_borders.top.width.calc_percent(parent_width);
    m_borders.bottom = m_css_borders.bottom.width.calc_percent(parent_width);
}

void HTMLElement::calc_auto_margins(int parent_width)
{
    if (get_element_position() != kPositionAbsolute &&
        (m_display == kDisplayBlock || m_display == kDisplayTable)) {
        if (m_css_margins.left.is_predefined() &&
            m_css_margins.right.is_predefined()) {
            int el_width = position_.width + m_borders.left + m_borders.right +
                           m_padding.left + m_padding.right;
            if (el_width <= parent_width) {
                m_margins.left = (parent_width - el_width) / 2;
                m_margins.right = (parent_width - el_width) - m_margins.left;
            } else {
                m_margins.left = 0;
                m_margins.right = 0;
            }
        } else if (m_css_margins.left.is_predefined() &&
                   !m_css_margins.right.is_predefined()) {
            int el_width = position_.width + m_borders.left + m_borders.right +
                           m_padding.left + m_padding.right + m_margins.right;
            m_margins.left = parent_width - el_width;
            if (m_margins.left < 0)
                m_margins.left = 0;
        } else if (!m_css_margins.left.is_predefined() &&
                   m_css_margins.right.is_predefined()) {
            int el_width = position_.width + m_borders.left + m_borders.right +
                           m_padding.left + m_padding.right + m_margins.left;
            m_margins.right = parent_width - el_width;
            if (m_margins.right < 0)
                m_margins.right = 0;
        }
    }
}

void HTMLElement::parse_attributes()
{
    for (auto& el : m_children) {
        el->parse_attributes();
    }
}

void HTMLElement::get_text(tstring& text) const
{
    for (auto& el : m_children) {
        el->get_text(text);
    }
}

bool HTMLElement::is_body() const
{
    return false;
}

void HTMLElement::set_data(const tchar_t*)
{
}

void HTMLElement::get_inline_boxes(std::vector<Position>& boxes)
{
    Box* old_box = nullptr;
    Position pos;
    for (auto& el : m_children) {
        if (!el->skip()) {
            if (el->m_box) {
                if (el->m_box != old_box) {
                    if (old_box) {
                        if (boxes.empty()) {
                            pos.x -= m_padding.left + m_borders.left;
                            pos.width += m_padding.left + m_borders.left;
                        }
                        boxes.push_back(pos);
                    }
                    old_box = el->m_box;
                    pos.x = el->left() + el->margin_left();
                    pos.y = el->top() - m_padding.top - m_borders.top;
                    pos.width = 0;
                    pos.height = 0;
                }
                pos.width =
                    el->right() - pos.x - el->margin_right() - el->margin_left();
                pos.height = std::max(pos.height,
                    el->height() + m_padding.top + m_padding.bottom +
                        m_borders.top + m_borders.bottom);
            } else if (el->get_display() == kDisplayInline) {
                std::vector<Position> sub_boxes;
                el->get_inline_boxes(sub_boxes);
                if (!sub_boxes.empty()) {
                    sub_boxes.rbegin()->width += el->margin_right();
                    if (boxes.empty()) {
                        if (m_padding.left + m_borders.left > 0) {
                            Position padding_box = (*sub_boxes.begin());
                            padding_box.x -= m_padding.left + m_borders.left +
                                             el->margin_left();
                            padding_box.width = m_padding.left + m_borders.left +
                                                el->margin_left();
                            boxes.push_back(padding_box);
                        }
                    }

                    sub_boxes.rbegin()->width += el->margin_right();

                    boxes.insert(boxes.end(), sub_boxes.begin(), sub_boxes.end());
                }
            }
        }
    }
    if (pos.width || pos.height) {
        if (boxes.empty()) {
            pos.x -= m_padding.left + m_borders.left;
            pos.width += m_padding.left + m_borders.left;
        }
        boxes.push_back(pos);
    }
    if (!boxes.empty()) {
        if (m_padding.right + m_borders.right > 0) {
            boxes.back().width += m_padding.right + m_borders.right;
        }
    }
}

bool HTMLElement::on_mouse_over()
{
    bool ret = false;

    Element::ptr el = this;
    while (el) {
        if (el->set_pseudo_class(_t("hover"), true)) {
            ret = true;
        }
        el = el->parent();
    }

    return ret;
}

bool HTMLElement::find_styles_changes(std::vector<Position>& redraw_boxes, int x, int y)
{
    if (m_display == kDisplayInlineText) {
        return false;
    }

    bool ret = false;
    bool apply = false;
    for (used_selector::vector::iterator iter = m_used_styles.begin();
         iter != m_used_styles.end() && !apply;
         iter++) {
        if ((*iter)->m_selector->is_media_valid()) {
            int res = select(*((*iter)->m_selector), true);
            if ((res == select_no_match && (*iter)->m_used) ||
                (res == select_match && !(*iter)->m_used)) {
                apply = true;
            }
        }
    }

    if (apply) {
        if (m_display == kDisplayInline || m_display == kDisplayTableRow) {
            std::vector<Position> boxes;
            get_inline_boxes(boxes);
            for (std::vector<Position>::iterator pos = boxes.begin();
                 pos != boxes.end();
                 pos++) {
                pos->x += x;
                pos->y += y;
                redraw_boxes.push_back(*pos);
            }
        } else {
            Position pos = position_;
            if (m_el_position != kPositionFixed) {
                pos.x += x;
                pos.y += y;
            }
            pos += m_padding;
            pos += m_borders;
            redraw_boxes.push_back(pos);
        }

        ret = true;
        refresh_styles();
        parse_styles();
    }
    for (auto& el : m_children) {
        if (!el->skip()) {
            if (m_el_position != kPositionFixed) {
                if (el->find_styles_changes(redraw_boxes, x + position_.x, y + position_.y)) {
                    ret = true;
                }
            } else {
                if (el->find_styles_changes(redraw_boxes, position_.x, position_.y)) {
                    ret = true;
                }
            }
        }
    }
    return ret;
}

bool HTMLElement::on_mouse_leave()
{
    bool ret = false;

    Element::ptr el = this;
    while (el) {
        if (el->set_pseudo_class(_t("hover"), false)) {
            ret = true;
        }
        if (el->set_pseudo_class(_t("active"), false)) {
            ret = true;
        }
        el = el->parent();
    }

    return ret;
}

bool HTMLElement::on_lbutton_down()
{
    bool ret = false;

    Element::ptr el = this;
    while (el) {
        if (el->set_pseudo_class(_t("active"), true)) {
            ret = true;
        }
        el = el->parent();
    }

    return ret;
}

bool HTMLElement::on_lbutton_up()
{
    bool ret = false;

    Element::ptr el = this;
    while (el) {
        if (el->set_pseudo_class(_t("active"), false)) {
            ret = true;
        }
        el = el->parent();
    }

    on_click();

    return ret;
}

void HTMLElement::on_click()
{
    if (have_parent()) {
        Element::ptr el_parent = parent();
        if (el_parent) {
            el_parent->on_click();
        }
    }
}

const tchar_t* HTMLElement::get_cursor()
{
    return get_style_property(kCSSPropertyCursor);
}

static const int font_size_table[8][7] = {{9, 9, 9, 9, 11, 14, 18},
    {9, 9, 9, 10, 12, 15, 20},
    {9, 9, 9, 11, 13, 17, 22},
    {9, 9, 10, 12, 14, 18, 24},
    {9, 9, 10, 13, 16, 20, 26},
    {9, 9, 11, 14, 17, 21, 28},
    {9, 10, 12, 15, 17, 23, 30},
    {9, 10, 13, 16, 18, 24, 32}};


void HTMLElement::init_font()
{
    // initialize font size
    const tchar_t* str = get_style_property(kCSSPropertyFontSize);

    int parent_sz = 0;
    int doc_font_size = get_document()->container()->get_default_font_size();
    Element::ptr el_parent = parent();
    if (el_parent) {
        parent_sz = el_parent->get_font_size();
    } else {
        parent_sz = doc_font_size;
    }


    if (!str) {
        font_size_ = parent_sz;
    } else {
        font_size_ = parent_sz;

        CSSLength sz;
        sz.parse_length_string(str, FONT_SIZE_STRINGS);
        if (sz.is_predefined()) {
            int idx_in_table = doc_font_size - 9;
            if (idx_in_table >= 0 && idx_in_table <= 7) {
                if (sz.predef() >= kFontSizeXXSmall &&
                    sz.predef() <= kFontSizeXXLarge) {
                    font_size_ = font_size_table[idx_in_table][sz.predef()];
                } else {
                    font_size_ = doc_font_size;
                }
            } else {
                switch (sz.predef()) {
                    case kFontSizeXXSmall:
                        font_size_ = doc_font_size * 3 / 5;
                        break;
                    case kFontSizeXSmall:
                        font_size_ = doc_font_size * 3 / 4;
                        break;
                    case kFontSizeSmall:
                        font_size_ = doc_font_size * 8 / 9;
                        break;
                    case kFontSizeLarge:
                        font_size_ = doc_font_size * 6 / 5;
                        break;
                    case kFontSizeXLarge:
                        font_size_ = doc_font_size * 3 / 2;
                        break;
                    case kFontSizeXXLarge:
                        font_size_ = doc_font_size * 2;
                        break;
                    default:
                        font_size_ = doc_font_size;
                        break;
                }
            }
        } else {
            if (sz.units() == kCSSUnitsPercent) {
                font_size_ = sz.calc_percent(parent_sz);
            } else if (sz.units() == kCSSUnitsNone) {
                font_size_ = parent_sz;
            } else {
                font_size_ = get_document()->cvt_units(sz, parent_sz);
            }
        }
    }

    // initialize font
    const tchar_t* name = get_style_property(kCSSPropertyFontFamily);
    const tchar_t* weight = get_style_property(kCSSPropertyFontWeight);
    const tchar_t* style = get_style_property(kCSSPropertyFontStyle);
    const tchar_t* decoration = get_style_property(kCSSPropertyTextDecoration);

    font_ = get_document()->get_font(name,
        font_size_,
        weight,
        style,
        decoration,
        &font_metrics_);
}

bool HTMLElement::is_break() const
{
    return false;
}

void HTMLElement::set_tagName(const tchar_t* tag)
{
    tstring s_val = tag;
    for (size_t i = 0; i < s_val.length(); i++) {
        s_val[i] = std::tolower(s_val[i], std::locale::classic());
    }
    m_tag = s_val;
}

void HTMLElement::draw_background(uintptr_t hdc, int x, int y, const Position* clip)
{
    Position pos = position_;
    pos.x += x;
    pos.y += y;

    Position el_pos = pos;
    el_pos += m_padding;
    el_pos += m_borders;

    if (m_display != kDisplayInline && m_display != kDisplayTableRow) {
        if (el_pos.does_intersect(clip)) {
            const Background* bg = get_background();
            if (bg) {
                BackgroundPaint bg_paint;
                init_BackgroundPaint(pos, bg_paint, bg);

                get_document()->container()->draw_background(hdc, bg_paint);
            }
            Position border_box = pos;
            border_box += m_padding;
            border_box += m_borders;

            Borders borders = m_css_borders.calculate_borders(border_box.width, border_box.height);
            get_document()->container()->draw_borders(hdc,
                borders,
                border_box,
                have_parent() ? false : true);
        }
    } else {
        const Background* bg = get_background();

        std::vector<Position> boxes;
        get_inline_boxes(boxes);

        BackgroundPaint bg_paint;
        Position content_box;

        for (std::vector<Position>::iterator box = boxes.begin(); box != boxes.end();
             box++) {
            box->x += x;
            box->y += y;

            if (box->does_intersect(clip)) {
                content_box = *box;
                content_box -= m_borders;
                content_box -= m_padding;

                if (bg) {
                    init_BackgroundPaint(content_box, bg_paint, bg);
                }

                CSSBorders css_borders;

                // set left borders radius for the first box
                if (box == boxes.begin()) {
                    css_borders.radii.top_left = m_css_borders.radii.top_left;
                    css_borders.radii.bottom_left = m_css_borders.radii.bottom_left;
                }

                // set right borders radius for the last box
                if (box == boxes.end() - 1) {
                    css_borders.radii.top_right = m_css_borders.radii.top_right;
                    css_borders.radii.bottom_right = m_css_borders.radii.bottom_right;
                }


                css_borders.top = m_css_borders.top;
                css_borders.bottom = m_css_borders.bottom;
                if (box == boxes.begin()) {
                    css_borders.left = m_css_borders.left;
                }
                if (box == boxes.end() - 1) {
                    css_borders.right = m_css_borders.right;
                }


                if (bg) {
                    bg_paint.border_radii = css_borders.radii.calculate_radii(bg_paint.border_box.width, bg_paint.border_box.width);
                    get_document()->container()->draw_background(hdc, bg_paint);
                }
                Borders borders = css_borders.calculate_borders(box->width, box->height);
                get_document()->container()->draw_borders(hdc, borders, *box, false);
            }
        }
    }
}

int HTMLElement::render_inline(const Element::ptr& container, int max_width)
{
    int ret_width = 0;
    int rw = 0;

    WhiteSpace ws = get_white_space();
    bool skip_spaces = false;
    if (ws == kWhiteSpaceNormal || ws == kWhiteSpaceNowrap ||
        ws == kWhiteSpacePreLine) {
        skip_spaces = true;
    }
    bool was_space = false;

    for (auto& el : m_children) {
        // skip spaces to make rendering a bit faster
        if (skip_spaces) {
            if (el->is_white_space()) {
                if (was_space) {
                    el->skip(true);
                    continue;
                } else {
                    was_space = true;
                }
            } else {
                was_space = false;
            }
        }

        rw = container->place_element(el, max_width);
        if (rw > ret_width) {
            ret_width = rw;
        }
    }
    return ret_width;
}

int HTMLElement::place_element(const Element::ptr& el, int max_width)
{
    if (el->get_display() == kDisplayNone)
        return 0;

    if (el->get_display() == kDisplayInline) {
        return el->render_inline(this, max_width);
    }

    ElementPosition el_position = el->get_element_position();

    if (el_position == kPositionAbsolute ||
        el_position == kPositionFixed) {
        int line_top = 0;
        if (!m_boxes.empty()) {
            if (m_boxes.back()->get_type() == kBoxLine) {
                line_top = m_boxes.back()->top();
                if (!m_boxes.back()->is_empty()) {
                    line_top += line_height();
                }
            } else {
                line_top = m_boxes.back()->bottom();
            }
        }

        el->render(0, line_top, max_width);
        el->position_.x += el->content_margins_left();
        el->position_.y += el->content_margins_top();

        return 0;
    }

    int ret_width = 0;

    switch (el->get_float()) {
        case kFloatLeft: {
            int line_top = 0;
            if (!m_boxes.empty()) {
                if (m_boxes.back()->get_type() == kBoxLine) {
                    line_top = m_boxes.back()->top();
                } else {
                    line_top = m_boxes.back()->bottom();
                }
            }
            line_top = get_cleared_top(el, line_top);
            int line_left = 0;
            int line_right = max_width;
            get_line_left_right(line_top, max_width, line_left, line_right);

            el->render(line_left, line_top, line_right);
            if (el->right() > line_right) {
                int new_top =
                    find_next_line_top(el->top(), el->width(), max_width);
                el->position_.x = get_line_left(new_top) + el->content_margins_left();
                el->position_.y = new_top + el->content_margins_top();
            }
            add_float(el, 0, 0);
            ret_width = fix_line_width(max_width, kFloatLeft);
            if (!ret_width) {
                ret_width = el->right();
            }
        } break;
        case kFloatRight: {
            int line_top = 0;
            if (!m_boxes.empty()) {
                if (m_boxes.back()->get_type() == kBoxLine) {
                    line_top = m_boxes.back()->top();
                } else {
                    line_top = m_boxes.back()->bottom();
                }
            }
            line_top = get_cleared_top(el, line_top);
            int line_left = 0;
            int line_right = max_width;
            get_line_left_right(line_top, max_width, line_left, line_right);

            el->render(0, line_top, line_right);

            if (line_left + el->width() > line_right) {
                int new_top =
                    find_next_line_top(el->top(), el->width(), max_width);
                el->position_.x = get_line_right(new_top, max_width) - el->width() +
                              el->content_margins_left();
                el->position_.y = new_top + el->content_margins_top();
            } else {
                el->position_.x =
                    line_right - el->width() + el->content_margins_left();
            }
            add_float(el, 0, 0);
            ret_width = fix_line_width(max_width, kFloatRight);

            if (!ret_width) {
                line_left = 0;
                line_right = max_width;
                get_line_left_right(line_top, max_width, line_left, line_right);

                ret_width = ret_width + (max_width - line_right);
            }
        } break;
        default: {
            line_context line_ctx;
            line_ctx.top = 0;
            if (!m_boxes.empty()) {
                line_ctx.top = m_boxes.back()->top();
            }
            line_ctx.left = 0;
            line_ctx.right = max_width;
            line_ctx.fix_top();
            get_line_left_right(line_ctx.top, max_width, line_ctx.left, line_ctx.right);

            switch (el->get_display()) {
                case kDisplayInlineBlock:
                case kDisplayInlineTable:
                    ret_width =
                        el->render(line_ctx.left, line_ctx.top, line_ctx.right);
                    break;
                case kDisplayBlock:
                    if (el->is_replaced() || el->is_floats_holder()) {
                        Element::ptr el_parent = el->parent();
                        el->position_.width = el->get_css_width().calc_percent(
                            line_ctx.right - line_ctx.left);
                        el->position_.height = el->get_css_height().calc_percent(
                            el_parent ? el_parent->position_.height : 0);
                    }
                    el->calc_outlines(line_ctx.right - line_ctx.left);
                    break;
                case kDisplayInlineText: {
                    Size sz;
                    el->get_content_size(sz, line_ctx.right);
                    el->position_ = sz;
                } break;
                default:
                    ret_width = 0;
                    break;
            }

            bool add_box = true;
            if (!m_boxes.empty()) {
                if (m_boxes.back()->can_hold(el, white_space_)) {
                    add_box = false;
                }
            }
            if (add_box) {
                new_box(el, max_width, line_ctx);
            } else if (!m_boxes.empty()) {
                line_ctx.top = m_boxes.back()->top();
            }

            if (line_ctx.top != line_ctx.calculatedTop) {
                line_ctx.left = 0;
                line_ctx.right = max_width;
                line_ctx.fix_top();
                get_line_left_right(line_ctx.top,
                    max_width,
                    line_ctx.left,
                    line_ctx.right);
            }

            if (!el->is_inline_box()) {
                if (m_boxes.size() == 1) {
                    if (collapse_top_margin()) {
                        int shift = el->margin_top();
                        if (shift >= 0) {
                            line_ctx.top -= shift;
                            m_boxes.back()->y_shift(-shift);
                        }
                    }
                } else {
                    int shift = 0;
                    int prev_margin = m_boxes[m_boxes.size() - 2]->bottom_margin();

                    if (prev_margin > el->margin_top()) {
                        shift = el->margin_top();
                    } else {
                        shift = prev_margin;
                    }
                    if (shift >= 0) {
                        line_ctx.top -= shift;
                        m_boxes.back()->y_shift(-shift);
                    }
                }
            }

            switch (el->get_display()) {
                case kDisplayTable:
                case kDisplayListItem:
                    ret_width =
                        el->render(line_ctx.left, line_ctx.top, line_ctx.width());
                    break;
                case kDisplayBlock:
                case kDisplayTableCell:
                case kDisplayTableCaption:
                case kDisplayTableRow:
                    if (el->is_replaced() || el->is_floats_holder()) {
                        ret_width = el->render(line_ctx.left,
                                        line_ctx.top,
                                        line_ctx.width()) +
                                    line_ctx.left + (max_width - line_ctx.right);
                    } else {
                        ret_width = el->render(0, line_ctx.top, max_width);
                    }
                    break;
                default:
                    ret_width = 0;
                    break;
            }

            m_boxes.back()->add_element(el);

            if (el->is_inline_box() && !el->skip()) {
                ret_width = el->right() + (max_width - line_ctx.right);
            }
        } break;
    }

    return ret_width;
}

bool HTMLElement::set_pseudo_class(const tchar_t* pclass, bool add)
{
    bool ret = false;
    if (add) {
        if (std::find(m_pseudo_classes.begin(), m_pseudo_classes.end(), pclass) ==
            m_pseudo_classes.end()) {
            m_pseudo_classes.push_back(pclass);
            ret = true;
        }
    } else {
        string_vector::iterator pi =
            std::find(m_pseudo_classes.begin(), m_pseudo_classes.end(), pclass);
        if (pi != m_pseudo_classes.end()) {
            m_pseudo_classes.erase(pi);
            ret = true;
        }
    }
    return ret;
}

bool HTMLElement::set_class(const tchar_t* pclass, bool add)
{
    string_vector classes;
    bool changed = false;

    split_string(pclass, classes, _t(" "));

    if (add) {
        for (auto& _class : classes) {
            if (std::find(m_class_values.begin(), m_class_values.end(), _class) ==
                m_class_values.end()) {
                m_class_values.push_back(std::move(_class));
                changed = true;
            }
        }
    } else {
        for (const auto& _class : classes) {
            auto end =
                std::remove(m_class_values.begin(), m_class_values.end(), _class);

            if (end != m_class_values.end()) {
                m_class_values.erase(end, m_class_values.end());
                changed = true;
            }
        }
    }

    if (changed) {
        tstring class_string;
        join_string(class_string, m_class_values, _t(" "));
        set_attr(_t("class"), class_string.c_str());

        return true;
    } else {
        return false;
    }
}

int HTMLElement::line_height() const
{
    return line_height_;
}

bool HTMLElement::is_replaced() const
{
    return false;
}

int HTMLElement::finish_last_box(bool end_of_render)
{
    int line_top = 0;

    if (!m_boxes.empty()) {
        m_boxes.back()->finish(end_of_render);

        if (m_boxes.back()->is_empty()) {
            line_top = m_boxes.back()->top();
            m_boxes.pop_back();
        }

        if (!m_boxes.empty()) {
            line_top = m_boxes.back()->bottom();
        }
    }
    return line_top;
}

int HTMLElement::new_box(const Element::ptr& el, int max_width, line_context& line_ctx)
{
    line_ctx.top = get_cleared_top(el, finish_last_box());

    line_ctx.left = 0;
    line_ctx.right = max_width;
    line_ctx.fix_top();
    get_line_left_right(line_ctx.top, max_width, line_ctx.left, line_ctx.right);

    if (el->is_inline_box() || el->is_floats_holder()) {
        if (el->width() > line_ctx.right - line_ctx.left) {
            line_ctx.top =
                find_next_line_top(line_ctx.top, el->width(), max_width);
            line_ctx.left = 0;
            line_ctx.right = max_width;
            line_ctx.fix_top();
            get_line_left_right(line_ctx.top, max_width, line_ctx.left, line_ctx.right);
        }
    }

    int first_line_margin = 0;
    if (m_boxes.empty() && list_style_type_ != kListStyleTypeNone &&
        list_style_position_ == kListStylePositionInside) {
        int sz_font = get_font_size();
        first_line_margin = sz_font;
    }

    if (el->is_inline_box()) {
        int text_indent = 0;
        if (m_css_text_indent.val() != 0.0f) {
            bool line_box_found = false;
            for (Box::vector::iterator iter = m_boxes.begin();
                 iter != m_boxes.end();
                 iter++) {
                if ((*iter)->get_type() == kBoxLine) {
                    line_box_found = true;
                    break;
                }
            }
            if (!line_box_found) {
                text_indent = m_css_text_indent.calc_percent(max_width);
            }
        }

        FontMetrics fm;
        get_font(&fm);
        m_boxes.emplace_back(std::unique_ptr<LineBox>(new LineBox(line_ctx.top,
            line_ctx.left + first_line_margin + text_indent,
            line_ctx.right,
            line_height(),
            fm,
            m_text_align)));
    } else {
        m_boxes.emplace_back(std::unique_ptr<BlockBox>(
            new BlockBox(line_ctx.top, line_ctx.left, line_ctx.right)));
    }

    return line_ctx.top;
}

int HTMLElement::get_cleared_top(const Element::ptr& el, int line_top) const
{
    switch (el->get_clear()) {
        case kClearLeft: {
            int fh = get_left_floats_height();
            if (fh && fh > line_top) {
                line_top = fh;
            }
        } break;
        case kClearRight: {
            int fh = get_right_floats_height();
            if (fh && fh > line_top) {
                line_top = fh;
            }
        } break;
        case kClearBoth: {
            int fh = get_floats_height();
            if (fh && fh > line_top) {
                line_top = fh;
            }
        } break;
        default:
            if (el->get_float() != kFloatNone) {
                int fh = get_floats_height(el->get_float());
                if (fh && fh > line_top) {
                    line_top = fh;
                }
            }
            break;
    }
    return line_top;
}

Display HTMLElement::get_display() const
{
    return m_display;
}

ElementFloat HTMLElement::get_float() const
{
    return m_float;
}

bool HTMLElement::is_floats_holder() const
{
    if (m_display == kDisplayInlineBlock || m_display == kDisplayTableCell ||
        !have_parent() || is_body() || m_float != kFloatNone ||
        m_el_position == kPositionAbsolute ||
        m_el_position == kPositionFixed || overflow_ > kOverflowVisible) {
        return true;
    }
    return false;
}

bool HTMLElement::is_first_child_inline(const Element::ptr& el) const
{
    if (!m_children.empty()) {
        for (const auto& this_el : m_children) {
            if (!this_el->is_white_space()) {
                if (el == this_el) {
                    return true;
                }
                if (this_el->get_display() == kDisplayInline) {
                    if (this_el->have_inline_child()) {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        }
    }
    return false;
}

bool HTMLElement::is_last_child_inline(const Element::ptr& el)
{
    if (!m_children.empty()) {
        for (auto this_el = m_children.rbegin(); this_el < m_children.rend();
             ++this_el) {
            if (!(*this_el)->is_white_space()) {
                if (el == (*this_el)) {
                    return true;
                }
                if ((*this_el)->get_display() == kDisplayInline) {
                    if ((*this_el)->have_inline_child()) {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        }
    }
    return false;
}

WhiteSpace HTMLElement::get_white_space() const
{
    return white_space_;
}

VerticalAlign HTMLElement::get_vertical_align() const
{
    return vertical_align_;
}

CSSLength HTMLElement::get_css_left() const
{
    return m_css_offsets.left;
}

CSSLength HTMLElement::get_css_right() const
{
    return m_css_offsets.right;
}

CSSLength HTMLElement::get_css_top() const
{
    return m_css_offsets.top;
}

CSSLength HTMLElement::get_css_bottom() const
{
    return m_css_offsets.bottom;
}


CSSOffsets HTMLElement::get_css_offsets() const
{
    return m_css_offsets;
}

ElementClear HTMLElement::get_clear() const
{
    return m_clear;
}

CSSLength HTMLElement::get_css_width() const
{
    return m_css_width;
}

CSSLength HTMLElement::get_css_height() const
{
    return m_css_height;
}

size_t HTMLElement::get_children_count() const
{
    return m_children.size();
}

Element::ptr HTMLElement::get_child(int idx) const
{
    return m_children[idx];
}

void HTMLElement::set_css_width(CSSLength& w)
{
    m_css_width = w;
}

void HTMLElement::apply_vertical_align()
{
    if (!m_boxes.empty()) {
        int add = 0;
        int content_height = m_boxes.back()->bottom();

        if (position_.height > content_height) {
            switch (vertical_align_) {
                case kVerticalAlignMiddle:
                    add = (position_.height - content_height) / 2;
                    break;
                case kVerticalAlignBottom:
                    add = position_.height - content_height;
                    break;
                default:
                    add = 0;
                    break;
            }
        }

        if (add) {
            for (size_t i = 0; i < m_boxes.size(); i++) {
                m_boxes[i]->y_shift(add);
            }
        }
    }
}

ElementPosition HTMLElement::get_element_position(CSSOffsets* offsets) const
{
    if (offsets && m_el_position != kPositionStatic) {
        *offsets = m_css_offsets;
    }
    return m_el_position;
}

void HTMLElement::init_BackgroundPaint(Position pos,
    BackgroundPaint& bg_paint,
    const Background* bg)
{
    if (!bg)
        return;

    bg_paint = *bg;
    Position content_box = pos;
    Position padding_box = pos;
    padding_box += m_padding;
    Position border_box = padding_box;
    border_box += m_borders;

    switch (bg->m_clip) {
        case kBackgroundBoxPaddingBox:
            bg_paint.clip_box = padding_box;
            break;
        case kBackgroundBoxContentBox:
            bg_paint.clip_box = content_box;
            break;
        default:
            bg_paint.clip_box = border_box;
            break;
    }

    switch (bg->m_origin) {
        case kBackgroundBoxBorderBox:
            bg_paint.origin_box = border_box;
            break;
        case kBackgroundBoxContentBox:
            bg_paint.origin_box = content_box;
            break;
        default:
            bg_paint.origin_box = padding_box;
            break;
    }

    if (!bg_paint.image.empty()) {
        bg_paint.image_size = get_document()->container()->get_image_size(bg_paint.image);
        if (bg_paint.image_size.width && bg_paint.image_size.height) {
            Size img_new_sz = bg_paint.image_size;
            double img_ar_width = (double)bg_paint.image_size.width /
                                  (double)bg_paint.image_size.height;
            double img_ar_height = (double)bg_paint.image_size.height /
                                   (double)bg_paint.image_size.width;


            if (bg->m_position.width.is_predefined()) {
                switch (bg->m_position.width.predef()) {
                    case background_size_contain:
                        if ((int)((double)bg_paint.origin_box.width *
                                  img_ar_height) <= bg_paint.origin_box.height) {
                            img_new_sz.width = bg_paint.origin_box.width;
                            img_new_sz.height =
                                (int)((double)bg_paint.origin_box.width *
                                      img_ar_height);
                        } else {
                            img_new_sz.height = bg_paint.origin_box.height;
                            img_new_sz.width =
                                (int)((double)bg_paint.origin_box.height *
                                      img_ar_width);
                        }
                        break;
                    case background_size_cover:
                        if ((int)((double)bg_paint.origin_box.width *
                                  img_ar_height) >= bg_paint.origin_box.height) {
                            img_new_sz.width = bg_paint.origin_box.width;
                            img_new_sz.height =
                                (int)((double)bg_paint.origin_box.width *
                                      img_ar_height);
                        } else {
                            img_new_sz.height = bg_paint.origin_box.height;
                            img_new_sz.width =
                                (int)((double)bg_paint.origin_box.height *
                                      img_ar_width);
                        }
                        break;

                    case background_size_auto:
                        if (!bg->m_position.height.is_predefined()) {
                            img_new_sz.height = bg->m_position.height.calc_percent(
                                bg_paint.origin_box.height);
                            img_new_sz.width =
                                (int)((double)img_new_sz.height * img_ar_width);
                        }
                        break;
                }
            } else {
                img_new_sz.width =
                    bg->m_position.width.calc_percent(bg_paint.origin_box.width);
                if (bg->m_position.height.is_predefined()) {
                    img_new_sz.height =
                        (int)((double)img_new_sz.width * img_ar_height);
                } else {
                    img_new_sz.height = bg->m_position.height.calc_percent(
                        bg_paint.origin_box.height);
                }
            }

            bg_paint.image_size = img_new_sz;
            bg_paint.position_x =
                bg_paint.origin_box.x +
                (int)bg->m_position.x.calc_percent(
                    bg_paint.origin_box.width - bg_paint.image_size.width);
            bg_paint.position_y =
                bg_paint.origin_box.y +
                (int)bg->m_position.y.calc_percent(
                    bg_paint.origin_box.height - bg_paint.image_size.height);
        }
    }
    bg_paint.border_radii = m_css_borders.radii.calculate_radii(border_box.width, border_box.height);
    ;
    bg_paint.border_box = border_box;
    bg_paint.is_root = have_parent() ? false : true;
}

Visibility HTMLElement::get_visibility() const
{
    return m_visibility;
}

void HTMLElement::draw_list_marker(uintptr_t hdc, const Position& pos)
{
    list_marker lm;

    const tchar_t* list_image = get_style_property(kCSSPropertyListStyleImage);
    Size img_size;
    if (list_image) {
        CSSStylesheet::parse_css_url(list_image, lm.image);
        lm.baseurl = get_style_property(kCSSPropertyListStyleImageBaseurl);
        URL list_image_url = resolve(URL(lm.baseurl), URL(lm.image));
        img_size = get_document()->container()->get_image_size(list_image_url);
    } else {
        lm.baseurl = nullptr;
    }

    int ln_height = line_height();
    int sz_font = get_font_size();
    lm.pos.x = pos.x;
    lm.pos.width = sz_font - sz_font * 2 / 3;
    lm.color = get_color(kCSSPropertyColor, Color(0, 0, 0));
    lm.marker_type = list_style_type_;
    lm.font = get_font();

    if (list_style_type_ >= kListStyleTypeArmenian) {
        lm.pos.y = pos.y;
        lm.pos.height = pos.height;
        lm.index = get_attr(_t("list_index"), _t(""))[0];
    } else {
        lm.pos.height = sz_font - sz_font * 2 / 3;
        lm.pos.y = pos.y + ln_height / 2 - lm.pos.height / 2;
        lm.index = -1;
    }

    if (img_size.width && img_size.height) {
        if (lm.pos.y + img_size.height > pos.y + pos.height) {
            lm.pos.y = pos.y + pos.height - img_size.height;
        }
        if (img_size.width > lm.pos.width) {
            lm.pos.x -= img_size.width - lm.pos.width;
        }

        lm.pos.width = img_size.width;
        lm.pos.height = img_size.height;
    }

    if (list_style_position_ == kListStylePositionOutside) {
        if (list_style_type_ >= kListStyleTypeArmenian) {
            auto tw_space =
                get_document()->container()->text_width(_t(" "), lm.font);
            lm.pos.x = pos.x - tw_space * 2;
            lm.pos.width = tw_space;
        } else {
            lm.pos.x -= sz_font;
        }
    }

    if (list_style_type_ >= kListStyleTypeArmenian) {
        auto marker_text = get_list_marker_text(lm.index);
        lm.pos.height = ln_height;
        if (marker_text.empty()) {
            get_document()->container()->draw_list_marker(hdc, lm);
        } else {
            marker_text += _t(".");
            auto tw = get_document()->container()->text_width(marker_text.c_str(),
                lm.font);
            auto text_pos = lm.pos;
            text_pos.move_to(text_pos.right() - tw, text_pos.y);
            text_pos.width = tw;
            get_document()->container()->draw_text(hdc,
                marker_text.c_str(),
                lm.font,
                lm.color,
                text_pos);
        }
    } else {
        get_document()->container()->draw_list_marker(hdc, lm);
    }
}

tstring HTMLElement::get_list_marker_text(int index)
{
    switch (list_style_type_) {
        case kListStyleTypeDecimal:
            return t_to_string(index);
        case kListStyleTypeDecimalLeadingZero: {
            auto txt = t_to_string(index);
            if (txt.length() == 1) {
                txt = _t("0") + txt;
            }
            return txt;
        }
        case kListStyleTypeLowerLatin:
        case kListStyleTypeLowerAlpha:
            return num_cvt::to_latin_lower(index);
        case kListStyleTypeLowerGreek:
            return num_cvt::to_greek_lower(index);
        case kListStyleTypeUpperAlpha:
        case kListStyleTypeUpperLatin:
            return num_cvt::to_latin_upper(index);
        case kListStyleTypeLowerRoman:
            return num_cvt::to_roman_lower(index);
        case kListStyleTypeUpperRoman:
            return num_cvt::to_roman_upper(index);
        case kListStyleTypeArmenian:
            break;
        case kListStyleTypeGeorgian:
            break;
        case kListStyleTypeHebrew:
            break;
        case kListStyleTypeHiragana:
            break;
        case kListStyleTypeHiraganaIroha:
            break;
        case kListStyleTypeKatakana:
            break;
        case kListStyleTypeKatakanaIroha:
            break;
        default:
            break;
    }
    return _t("");
}

void HTMLElement::draw_children(uintptr_t hdc,
    int x,
    int y,
    const Position* clip,
    DrawFlag flag,
    int zindex)
{
    if (m_display == kDisplayTable || m_display == kDisplayInlineTable) {
        draw_children_table(hdc, x, y, clip, flag, zindex);
    } else {
        draw_children_box(hdc, x, y, clip, flag, zindex);
    }
}

bool HTMLElement::fetch_positioned()
{
    bool ret = false;

    m_positioned.clear();

    ElementPosition el_pos;

    for (auto& el : m_children) {
        el_pos = el->get_element_position();
        if (el_pos != kPositionStatic) {
            add_positioned(el);
        }
        if (!ret && (el_pos == kPositionAbsolute ||
                        el_pos == kPositionFixed)) {
            ret = true;
        }
        if (el->fetch_positioned()) {
            ret = true;
        }
    }
    return ret;
}

int HTMLElement::get_zindex() const
{
    return m_z_index;
}

void HTMLElement::render_positioned(RenderType rt)
{
    Position wnd_position;
    get_document()->container()->get_client_rect(wnd_position);

    ElementPosition el_position;
    bool process;
    for (auto& el : m_positioned) {
        el_position = el->get_element_position();

        process = false;
        if (el->get_display() != kDisplayNone) {
            if (el_position == kPositionAbsolute) {
                if (rt != kRenderFixedOnly) {
                    process = true;
                }
            } else if (el_position == kPositionFixed) {
                if (rt != kRenderNoFixed) {
                    process = true;
                }
            }
        }

        if (process) {
            int parent_height = 0;
            int parent_width = 0;
            //int client_x = 0;
            //int client_y = 0;
            if (el_position == kPositionFixed) {
                parent_height = wnd_position.height;
                parent_width = wnd_position.width;
                //client_x = wnd_position.left();
                //client_y = wnd_position.top();
            } else {
                Element::ptr el_parent = el->parent();
                if (el_parent) {
                    parent_height = el_parent->height();
                    parent_width = el_parent->width();
                }
            }

            CSSLength css_left = el->get_css_left();
            CSSLength css_right = el->get_css_right();
            CSSLength css_top = el->get_css_top();
            CSSLength css_bottom = el->get_css_bottom();

            bool need_render = false;

            CSSLength el_w = el->get_css_width();
            CSSLength el_h = el->get_css_height();

            int new_width = -1;
            int new_height = -1;
            if (el_w.units() == kCSSUnitsPercent && parent_width) {
                new_width = el_w.calc_percent(parent_width);
                if (el->position_.width != new_width) {
                    need_render = true;
                    el->position_.width = new_width;
                }
            }

            if (el_h.units() == kCSSUnitsPercent && parent_height) {
                new_height = el_h.calc_percent(parent_height);
                if (el->position_.height != new_height) {
                    need_render = true;
                    el->position_.height = new_height;
                }
            }

            bool cvt_x = false;
            bool cvt_y = false;

            if (el_position == kPositionFixed) {
                if (!css_left.is_predefined() || !css_right.is_predefined()) {
                    if (!css_left.is_predefined() && css_right.is_predefined()) {
                        el->position_.x = css_left.calc_percent(parent_width) +
                                      el->content_margins_left();
                    } else if (css_left.is_predefined() &&
                               !css_right.is_predefined()) {
                        el->position_.x =
                            parent_width - css_right.calc_percent(parent_width) -
                            el->position_.width - el->content_margins_right();
                    } else {
                        el->position_.x = css_left.calc_percent(parent_width) +
                                      el->content_margins_left();
                        el->position_.width = parent_width -
                                          css_left.calc_percent(parent_width) -
                                          css_right.calc_percent(parent_width) -
                                          (el->content_margins_left() +
                                              el->content_margins_right());
                        need_render = true;
                    }
                }

                if (!css_top.is_predefined() || !css_bottom.is_predefined()) {
                    if (!css_top.is_predefined() && css_bottom.is_predefined()) {
                        el->position_.y = css_top.calc_percent(parent_height) +
                                      el->content_margins_top();
                    } else if (css_top.is_predefined() &&
                               !css_bottom.is_predefined()) {
                        el->position_.y = parent_height -
                                      css_bottom.calc_percent(parent_height) -
                                      el->position_.height -
                                      el->content_margins_bottom();
                    } else {
                        el->position_.y = css_top.calc_percent(parent_height) +
                                      el->content_margins_top();
                        el->position_.height =
                            parent_height - css_top.calc_percent(parent_height) -
                            css_bottom.calc_percent(parent_height) -
                            (el->content_margins_top() +
                                el->content_margins_bottom());
                        need_render = true;
                    }
                }
            } else {
                if (!css_left.is_predefined() || !css_right.is_predefined()) {
                    if (!css_left.is_predefined() && css_right.is_predefined()) {
                        el->position_.x = css_left.calc_percent(parent_width) +
                                      el->content_margins_left() - m_padding.left;
                    } else if (css_left.is_predefined() &&
                               !css_right.is_predefined()) {
                        el->position_.x = position_.width + m_padding.right -
                                      css_right.calc_percent(parent_width) -
                                      el->position_.width -
                                      el->content_margins_right();
                    } else {
                        el->position_.x = css_left.calc_percent(parent_width) +
                                      el->content_margins_left() - m_padding.left;
                        el->position_.width = position_.width + m_padding.left +
                                          m_padding.right -
                                          css_left.calc_percent(parent_width) -
                                          css_right.calc_percent(parent_width) -
                                          (el->content_margins_left() +
                                              el->content_margins_right());
                        if (new_width != -1) {
                            el->position_.x += (el->position_.width - new_width) / 2;
                            el->position_.width = new_width;
                        }
                        need_render = true;
                    }
                    cvt_x = true;
                }

                if (!css_top.is_predefined() || !css_bottom.is_predefined()) {
                    if (!css_top.is_predefined() && css_bottom.is_predefined()) {
                        el->position_.y = css_top.calc_percent(parent_height) +
                                      el->content_margins_top() - m_padding.top;
                    } else if (css_top.is_predefined() &&
                               !css_bottom.is_predefined()) {
                        el->position_.y = position_.height + m_padding.bottom -
                                      css_bottom.calc_percent(parent_height) -
                                      el->position_.height -
                                      el->content_margins_bottom();
                    } else {
                        el->position_.y = css_top.calc_percent(parent_height) +
                                      el->content_margins_top() - m_padding.top;
                        el->position_.height =
                            position_.height + m_padding.top + m_padding.bottom -
                            css_top.calc_percent(parent_height) -
                            css_bottom.calc_percent(parent_height) -
                            (el->content_margins_top() +
                                el->content_margins_bottom());
                        if (new_height != -1) {
                            el->position_.y += (el->position_.height - new_height) / 2;
                            el->position_.height = new_height;
                        }
                        need_render = true;
                    }
                    cvt_y = true;
                }
            }

            if (cvt_x || cvt_y) {
                int offset_x = 0;
                int offset_y = 0;
                Element::ptr cur_el = el->parent();
                Element::ptr this_el = this;
                while (cur_el && cur_el != this_el) {
                    offset_x += cur_el->position_.x;
                    offset_y += cur_el->position_.y;
                    cur_el = cur_el->parent();
                }
                if (cvt_x)
                    el->position_.x -= offset_x;
                if (cvt_y)
                    el->position_.y -= offset_y;
            }

            if (need_render) {
                Position pos = el->position_;
                el->render(el->left(), el->top(), el->width(), true);
                el->position_ = pos;
            }

            if (el_position == kPositionFixed) {
                Position fixed_pos;
                el->get_redraw_box(fixed_pos);
                get_document()->add_fixed_box(fixed_pos);
            }
        }

        el->render_positioned();
    }

    if (!m_positioned.empty()) {
        std::stable_sort(m_positioned.begin(),
            m_positioned.end(),
            [](const Element::ptr& _Left, const Element::ptr& _Right) {
                return (_Left->get_zindex() < _Right->get_zindex());
            });
    }
}

void HTMLElement::draw_stacking_context(uintptr_t hdc,
    int x,
    int y,
    const Position* clip,
    bool with_positioned)
{
    if (!is_visible())
        return;

    std::map<int, bool> zindexes;
    if (with_positioned) {
        for (ElementsVector::iterator i = m_positioned.begin();
             i != m_positioned.end();
             i++) {
            zindexes[(*i)->get_zindex()];
        }

        for (std::map<int, bool>::iterator idx = zindexes.begin();
             idx != zindexes.end();
             idx++) {
            if (idx->first < 0) {
                draw_children(hdc, x, y, clip, kDrawPositioned, idx->first);
            }
        }
    }
    draw_children(hdc, x, y, clip, kDrawBlock, 0);
    draw_children(hdc, x, y, clip, kDrawFloats, 0);
    draw_children(hdc, x, y, clip, kDrawInlines, 0);
    if (with_positioned) {
        for (std::map<int, bool>::iterator idx = zindexes.begin();
             idx != zindexes.end();
             idx++) {
            if (idx->first == 0) {
                draw_children(hdc, x, y, clip, kDrawPositioned, idx->first);
            }
        }

        for (std::map<int, bool>::iterator idx = zindexes.begin();
             idx != zindexes.end();
             idx++) {
            if (idx->first > 0) {
                draw_children(hdc, x, y, clip, kDrawPositioned, idx->first);
            }
        }
    }
}

Overflow HTMLElement::get_overflow() const
{
    return overflow_;
}

bool HTMLElement::is_nth_child(const Element::ptr& el, int num, int off, bool of_type) const
{
    int idx = 1;
    for (const auto& child : m_children) {
        if (child->get_display() != kDisplayInlineText) {
            if ((!of_type) ||
                (of_type && !t_strcmp(el->get_tagName(), child->get_tagName()))) {
                if (el == child) {
                    if (num != 0) {
                        if ((idx - off) >= 0 && (idx - off) % num == 0) {
                            return true;
                        }

                    } else if (idx == off) {
                        return true;
                    }
                    return false;
                }
                idx++;
            }
            if (el == child)
                break;
        }
    }
    return false;
}

bool HTMLElement::is_nth_last_child(const Element::ptr& el,
    int num,
    int off,
    bool of_type) const
{
    int idx = 1;
    for (ElementsVector::const_reverse_iterator child = m_children.rbegin();
         child != m_children.rend();
         child++) {
        if ((*child)->get_display() != kDisplayInlineText) {
            if (!of_type || (of_type && !t_strcmp(el->get_tagName(),
                                            (*child)->get_tagName()))) {
                if (el == (*child)) {
                    if (num != 0) {
                        if ((idx - off) >= 0 && (idx - off) % num == 0) {
                            return true;
                        }

                    } else if (idx == off) {
                        return true;
                    }
                    return false;
                }
                idx++;
            }
            if (el == (*child))
                break;
        }
    }
    return false;
}

void HTMLElement::parse_nth_child_params(tstring param, int& num, int& off)
{
    if (param == _t("odd")) {
        num = 2;
        off = 1;
    } else if (param == _t("even")) {
        num = 2;
        off = 0;
    } else {
        string_vector tokens;
        split_string(param, tokens, _t(" n"), _t("n"));

        tstring s_num;
        tstring s_off;

        tstring s_int;
        for (string_vector::iterator tok = tokens.begin(); tok != tokens.end();
             tok++) {
            if ((*tok) == _t("n")) {
                s_num = s_int;
                s_int.clear();
            } else {
                s_int += (*tok);
            }
        }
        s_off = s_int;

        num = t_atoi(s_num.c_str());
        off = t_atoi(s_off.c_str());
    }
}

void HTMLElement::calc_document_size(Size& sz, int x /*= 0*/, int y /*= 0*/)
{
    if (is_visible() && m_el_position != kPositionFixed) {
        Element::calc_document_size(sz, x, y);

        if (overflow_ == kOverflowVisible) {
            for (auto& el : m_children) {
                el->calc_document_size(sz, x + position_.x, y + position_.y);
            }
        }

        // root element (<html>) must to cover entire window
        if (!have_parent()) {
            Position client_pos;
            get_document()->container()->get_client_rect(client_pos);
            position_.height = std::max(sz.height, client_pos.height) -
                           content_margins_top() - content_margins_bottom();
            position_.width = std::max(sz.width, client_pos.width) -
                          content_margins_left() - content_margins_right();
        }
    }
}


void HTMLElement::get_redraw_box(Position& pos, int x /*= 0*/, int y /*= 0*/)
{
    if (is_visible()) {
        Element::get_redraw_box(pos, x, y);

        if (overflow_ == kOverflowVisible) {
            for (auto& el : m_children) {
                if (el->get_element_position() != kPositionFixed) {
                    el->get_redraw_box(pos, x + position_.x, y + position_.y);
                }
            }
        }
    }
}

Element::ptr HTMLElement::find_adjacent_sibling(const Element::ptr& el,
    const CSSSelector& selector,
    bool apply_pseudo /*= true*/,
    bool* is_pseudo /*= 0*/)
{
    Element* ret = nullptr;
    for (auto& e : m_children) {
        if (e->get_display() != kDisplayInlineText) {
            if (e == el) {
                if (ret) {
                    int res = ret->select(selector, apply_pseudo);
                    if (res != select_no_match) {
                        if (is_pseudo) {
                            if (res & select_match_pseudo_class) {
                                *is_pseudo = true;
                            } else {
                                *is_pseudo = false;
                            }
                        }
                        return ret;
                    }
                }
                return nullptr;
            } else {
                ret = e;
            }
        }
    }
    return nullptr;
}

Element::ptr HTMLElement::find_sibling(const Element::ptr& el,
    const CSSSelector& selector,
    bool apply_pseudo /*= true*/,
    bool* is_pseudo /*= 0*/)
{
    Element::ptr ret = nullptr;
    for (auto& e : m_children) {
        if (e->get_display() != kDisplayInlineText) {
            if (e == el) {
                return ret;
            } else if (!ret) {
                int res = e->select(selector, apply_pseudo);
                if (res != select_no_match) {
                    if (is_pseudo) {
                        if (res & select_match_pseudo_class) {
                            *is_pseudo = true;
                        } else {
                            *is_pseudo = false;
                        }
                    }
                    ret = e;
                }
            }
        }
    }
    return nullptr;
}

bool HTMLElement::is_only_child(const Element::ptr& el, bool of_type) const
{
    int child_count = 0;
    for (const auto& child : m_children) {
        if (child->get_display() != kDisplayInlineText) {
            if (!of_type ||
                (of_type && !t_strcmp(el->get_tagName(), child->get_tagName()))) {
                child_count++;
            }
            if (child_count > 1)
                break;
        }
    }
    if (child_count > 1) {
        return false;
    }
    return true;
}

void HTMLElement::update_floats(int dy, const Element::ptr& parent)
{
    if (is_floats_holder()) {
        bool reset_cache = false;
        for (floated_box::vector::reverse_iterator fb = m_floats_left.rbegin();
             fb != m_floats_left.rend();
             fb++) {
            if (fb->el->is_ancestor(parent)) {
                reset_cache = true;
                fb->pos.y += dy;
            }
        }
        if (reset_cache) {
            m_cahe_line_left.invalidate();
        }
        reset_cache = false;
        for (floated_box::vector::reverse_iterator fb = m_floats_right.rbegin();
             fb != m_floats_right.rend();
             fb++) {
            if (fb->el->is_ancestor(parent)) {
                reset_cache = true;
                fb->pos.y += dy;
            }
        }
        if (reset_cache) {
            m_cahe_line_right.invalidate();
        }
    } else {
        Element::ptr el_parent = this->parent();
        if (el_parent) {
            el_parent->update_floats(dy, parent);
        }
    }
}

void HTMLElement::remove_before_after()
{
    if (!m_children.empty()) {
        if (!t_strcmp(m_children.front()->get_tagName(), _t("::before"))) {
            m_children.erase(m_children.begin());
        }
    }
    if (!m_children.empty()) {
        if (!t_strcmp(m_children.back()->get_tagName(), _t("::after"))) {
            m_children.erase(m_children.end() - 1);
        }
    }
}

Element::ptr HTMLElement::get_element_before()
{
    if (!m_children.empty()) {
        if (!t_strcmp(m_children.front()->get_tagName(), _t("::before"))) {
            return m_children.front();
        }
    }
    Element* el = new BeforeElement(get_document());
    el->parent(this);
    m_children.insert(m_children.begin(), el);
    return el;
}

Element::ptr HTMLElement::get_element_after()
{
    if (!m_children.empty()) {
        if (!t_strcmp(m_children.back()->get_tagName(), _t("::after"))) {
            return m_children.back();
        }
    }
    Element* element = new AfterElement(get_document());
    append_child(element);
    return element;
}

void HTMLElement::add_style(const CSSStyle& st)
{
    m_style.combine(st);
}

bool HTMLElement::have_inline_child() const
{
    if (!m_children.empty()) {
        for (const auto& el : m_children) {
            if (!el->is_white_space()) {
                return true;
            }
        }
    }
    return false;
}

void HTMLElement::refresh_styles()
{
    remove_before_after();

    for (auto& el : m_children) {
        if (el->get_display() != kDisplayInlineText) {
            el->refresh_styles();
        }
    }

    m_style.clear();

    for (auto& usel : m_used_styles) {
        usel->m_used = false;

        if (usel->m_selector->is_media_valid()) {
            int apply = select(*usel->m_selector, false);

            if (apply != select_no_match) {
                if (apply & select_match_pseudo_class) {
                    if (select(*usel->m_selector, true)) {
                        if (apply & select_match_with_after) {
                            Element::ptr el = get_element_after();
                            if (el) {
                                el->add_style(*usel->m_selector->m_style);
                            }
                        } else if (apply & select_match_with_before) {
                            Element::ptr el = get_element_before();
                            if (el) {
                                el->add_style(*usel->m_selector->m_style);
                            }
                        } else {
                            add_style(*usel->m_selector->m_style);
                            usel->m_used = true;
                        }
                    }
                } else if (apply & select_match_with_after) {
                    Element::ptr el = get_element_after();
                    if (el) {
                        el->add_style(*usel->m_selector->m_style);
                    }
                } else if (apply & select_match_with_before) {
                    Element::ptr el = get_element_before();
                    if (el) {
                        el->add_style(*usel->m_selector->m_style);
                    }
                } else {
                    add_style(*usel->m_selector->m_style);
                    usel->m_used = true;
                }
            }
        }
    }
}

Element::ptr HTMLElement::get_child_by_point(int x,
    int y,
    int client_x,
    int client_y,
    DrawFlag flag,
    int zindex)
{
    Element::ptr ret = nullptr;

    if (overflow_ > kOverflowVisible) {
        if (!position_.is_point_inside(x, y)) {
            return ret;
        }
    }

    Position pos = position_;
    pos.x = x - pos.x;
    pos.y = y - pos.y;

    for (ElementsVector::reverse_iterator i = m_children.rbegin();
         i != m_children.rend() && !ret;
         i++) {
        Element::ptr el = (*i);

        if (el->is_visible() && el->get_display() != kDisplayInlineText) {
            switch (flag) {
                case kDrawPositioned:
                    if (el->is_positioned() && el->get_zindex() == zindex) {
                        if (el->get_element_position() == kPositionFixed) {
                            ret = el->get_element_by_point(client_x,
                                client_y,
                                client_x,
                                client_y);
                            if (!ret && (*i)->is_point_inside(client_x, client_y)) {
                                ret = (*i);
                            }
                        } else {
                            ret = el->get_element_by_point(pos.x,
                                pos.y,
                                client_x,
                                client_y);
                            if (!ret && (*i)->is_point_inside(pos.x, pos.y)) {
                                ret = (*i);
                            }
                        }
                        el = nullptr;
                    }
                    break;
                case kDrawBlock:
                    if (!el->is_inline_box() && el->get_float() == kFloatNone &&
                        !el->is_positioned()) {
                        if (el->is_point_inside(pos.x, pos.y)) {
                            ret = el;
                        }
                    }
                    break;
                case kDrawFloats:
                    if (el->get_float() != kFloatNone && !el->is_positioned()) {
                        ret = el->get_element_by_point(pos.x, pos.y, client_x, client_y);

                        if (!ret && (*i)->is_point_inside(pos.x, pos.y)) {
                            ret = (*i);
                        }
                        el = nullptr;
                    }
                    break;
                case kDrawInlines:
                    if (el->is_inline_box() && el->get_float() == kFloatNone &&
                        !el->is_positioned()) {
                        if (el->get_display() == kDisplayInlineBlock) {
                            ret = el->get_element_by_point(pos.x,
                                pos.y,
                                client_x,
                                client_y);
                            el = nullptr;
                        }
                        if (!ret && (*i)->is_point_inside(pos.x, pos.y)) {
                            ret = (*i);
                        }
                    }
                    break;
                default:
                    break;
            }

            if (el && !el->is_positioned()) {
                if (flag == kDrawPositioned) {
                    Element::ptr child = el->get_child_by_point(pos.x,
                        pos.y,
                        client_x,
                        client_y,
                        flag,
                        zindex);
                    if (child) {
                        ret = child;
                    }
                } else {
                    if (el->get_float() == kFloatNone &&
                        el->get_display() != kDisplayInlineBlock) {
                        Element::ptr child = el->get_child_by_point(pos.x,
                            pos.y,
                            client_x,
                            client_y,
                            flag,
                            zindex);
                        if (child) {
                            ret = child;
                        }
                    }
                }
            }
        }
    }

    return ret;
}

Element::ptr HTMLElement::get_element_by_point(int x, int y, int client_x, int client_y)
{
    if (!is_visible()) {
        return nullptr;
    }

    Element* ret = nullptr;

    std::map<int, bool> zindexes;

    for (ElementsVector::iterator i = m_positioned.begin();
         i != m_positioned.end();
         i++) {
        zindexes[(*i)->get_zindex()];
    }

    for (std::map<int, bool>::iterator idx = zindexes.begin();
         idx != zindexes.end() && !ret;
         idx++) {
        if (idx->first > 0) {
            ret =
                get_child_by_point(x, y, client_x, client_y, kDrawPositioned, idx->first);
        }
    }
    if (ret)
        return ret;

    for (std::map<int, bool>::iterator idx = zindexes.begin();
         idx != zindexes.end() && !ret;
         idx++) {
        if (idx->first == 0) {
            ret =
                get_child_by_point(x, y, client_x, client_y, kDrawPositioned, idx->first);
        }
    }
    if (ret)
        return ret;

    ret = get_child_by_point(x, y, client_x, client_y, kDrawInlines, 0);
    if (ret)
        return ret;

    ret = get_child_by_point(x, y, client_x, client_y, kDrawFloats, 0);
    if (ret)
        return ret;

    ret = get_child_by_point(x, y, client_x, client_y, kDrawBlock, 0);
    if (ret)
        return ret;


    for (std::map<int, bool>::iterator idx = zindexes.begin();
         idx != zindexes.end() && !ret;
         idx++) {
        if (idx->first < 0) {
            ret =
                get_child_by_point(x, y, client_x, client_y, kDrawPositioned, idx->first);
        }
    }
    if (ret)
        return ret;

    if (m_el_position == kPositionFixed) {
        if (is_point_inside(client_x, client_y)) {
            ret = this;
        }
    } else {
        if (is_point_inside(x, y)) {
            ret = this;
        }
    }

    return ret;
}

const Background* HTMLElement::get_background(bool own_only)
{
    if (own_only) {
        // return own background with check for empty one
        if (m_bg.m_image.empty() && !m_bg.m_color.alpha) {
            return nullptr;
        }
        return &m_bg;
    }

    if (m_bg.m_image.empty() && !m_bg.m_color.alpha) {
        // if this is root element (<html>) try to get background from body
        if (!have_parent()) {
            for (const auto& el : m_children) {
                if (el->is_body()) {
                    // return own body background
                    return el->get_background(true);
                }
            }
        }
        return nullptr;
    }

    if (is_body()) {
        Element::ptr el_parent = parent();
        if (el_parent) {
            if (!el_parent->get_background(true)) {
                // parent of body will draw background for body
                return nullptr;
            }
        }
    }

    return &m_bg;
}

int HTMLElement::render_box(int x, int y, int max_width, bool second_pass /*= false*/)
{
    int parent_width = max_width;

    calc_outlines(parent_width);

    position_.clear();
    position_.move_to(x, y);

    position_.x += content_margins_left();
    position_.y += content_margins_top();

    int ret_width = 0;

    def_value<int> block_width(0);

    if (m_display != kDisplayTableCell && !m_css_width.is_predefined()) {
        int w = calc_width(parent_width);

        if (box_sizing_ == kBoxSizingBorderBox) {
            w -= m_padding.width() + m_borders.width();
        }
        ret_width = max_width = block_width = w;
    } else {
        if (max_width) {
            max_width -= content_margins_left() + content_margins_right();
        }
    }

    // check for max-width (on the first pass only)
    if (!m_css_max_width.is_predefined() && !second_pass) {
        int mw =
            get_document()->cvt_units(m_css_max_width, font_size_, parent_width);
        if (box_sizing_ == kBoxSizingBorderBox) {
            mw -= m_padding.left + m_borders.left + m_padding.right +
                  m_borders.right;
        }
        if (max_width > mw) {
            max_width = mw;
        }
    }

    m_floats_left.clear();
    m_floats_right.clear();
    m_boxes.clear();
    m_cahe_line_left.invalidate();
    m_cahe_line_right.invalidate();

    ElementPosition el_position;

    int block_height = 0;

    position_.height = 0;

    if (get_predefined_height(block_height)) {
        position_.height = block_height;
    }

    WhiteSpace ws = get_white_space();
    bool skip_spaces = false;
    if (ws == kWhiteSpaceNormal || ws == kWhiteSpaceNowrap ||
        ws == kWhiteSpacePreLine) {
        skip_spaces = true;
    }

    bool was_space = false;

    for (auto el : m_children) {
        // we don't need process absolute and fixed positioned element on the second pass
        if (second_pass) {
            el_position = el->get_element_position();
            if ((el_position == kPositionAbsolute ||
                    el_position == kPositionFixed))
                continue;
        }

        // skip spaces to make rendering a bit faster
        if (skip_spaces) {
            if (el->is_white_space()) {
                if (was_space) {
                    el->skip(true);
                    continue;
                } else {
                    was_space = true;
                }
            } else {
                was_space = false;
            }
        }

        // place element into rendering flow
        int rw = place_element(el, max_width);
        if (rw > ret_width) {
            ret_width = rw;
        }
    }

    finish_last_box(true);

    if (block_width.is_default() && is_inline_box()) {
        position_.width = ret_width;
    } else {
        position_.width = max_width;
    }
    calc_auto_margins(parent_width);

    if (!m_boxes.empty()) {
        if (collapse_top_margin()) {
            int old_top = m_margins.top;
            m_margins.top = std::max(m_boxes.front()->top_margin(), m_margins.top);
            if (m_margins.top != old_top) {
                update_floats(m_margins.top - old_top, this);
            }
        }
        if (collapse_bottom_margin()) {
            m_margins.bottom =
                std::max(m_boxes.back()->bottom_margin(), m_margins.bottom);
            position_.height =
                m_boxes.back()->bottom() - m_boxes.back()->bottom_margin();
        } else {
            position_.height = m_boxes.back()->bottom();
        }
    }

    // add the floats height to the block height
    if (is_floats_holder()) {
        int floats_height = get_floats_height();
        if (floats_height > position_.height) {
            position_.height = floats_height;
        }
    }

    // calculate the final position

    position_.move_to(x, y);
    position_.x += content_margins_left();
    position_.y += content_margins_top();

    if (get_predefined_height(block_height)) {
        position_.height = block_height;
    }

    int min_height = 0;
    if (!m_css_min_height.is_predefined() &&
        m_css_min_height.units() == kCSSUnitsPercent) {
        Element::ptr el_parent = parent();
        if (el_parent) {
            if (el_parent->get_predefined_height(block_height)) {
                min_height = m_css_min_height.calc_percent(block_height);
            }
        }
    } else {
        min_height = (int)m_css_min_height.val();
    }
    if (min_height != 0 && box_sizing_ == kBoxSizingBorderBox) {
        min_height -=
            m_padding.top + m_borders.top + m_padding.bottom + m_borders.bottom;
        if (min_height < 0)
            min_height = 0;
    }

    if (m_display == kDisplayListItem) {
        const tchar_t* list_image = get_style_property(kCSSPropertyListStyleImage);
        if (list_image) {
            tstring url;
            CSSStylesheet::parse_css_url(list_image, url);

            const tchar_t* list_image_baseurl =
                get_style_property(kCSSPropertyListStyleImageBaseurl);

            URL list_image_url = resolve(URL(list_image_baseurl), URL(url));
            Size sz = get_document()->container()->get_image_size(list_image_url);

            if (min_height < sz.height) {
                min_height = sz.height;
            }
        }
    }

    if (min_height > position_.height) {
        position_.height = min_height;
    }

    int min_width = m_css_min_width.calc_percent(parent_width);

    if (min_width != 0 && box_sizing_ == kBoxSizingBorderBox) {
        min_width -=
            m_padding.left + m_borders.left + m_padding.right + m_borders.right;
        if (min_width < 0)
            min_width = 0;
    }

    if (min_width != 0) {
        if (min_width > position_.width) {
            position_.width = min_width;
        }
        if (min_width > ret_width) {
            ret_width = min_width;
        }
    }

    ret_width += content_margins_left() + content_margins_right();

    // re-render with new width
    if (ret_width < max_width && !second_pass && have_parent()) {
        if (m_display == kDisplayInlineBlock ||
            (m_css_width.is_predefined() &&
                (m_float != kFloatNone || m_display == kDisplayTable ||
                    m_el_position == kPositionAbsolute ||
                    m_el_position == kPositionFixed))) {
            render(x, y, ret_width, true);
            position_.width =
                ret_width - (content_margins_left() + content_margins_right());
        }
    }

    if (is_floats_holder() && !second_pass) {
        for (const auto& fb : m_floats_left) {
            fb.el->apply_relative_shift(fb.el->parent()->calc_width(position_.width));
        }
    }


    return ret_width;
}

int HTMLElement::render_table(int x, int y, int max_width, bool /* second_pass *//*= false*/)
{
    if (!m_grid)
        return 0;

    int parent_width = max_width;

    calc_outlines(parent_width);

    position_.clear();
    position_.move_to(x, y);

    position_.x += content_margins_left();
    position_.y += content_margins_top();

    def_value<int> block_width(0);

    if (!m_css_width.is_predefined()) {
        max_width = block_width =
            calc_width(parent_width) - m_padding.width() - m_borders.width();
    } else {
        if (max_width) {
            max_width -= content_margins_left() + content_margins_right();
        }
    }

    // Calculate table spacing
    int table_width_spacing = 0;
    if (m_border_collapse == border_collapse_separate) {
        table_width_spacing = m_border_spacing_x * (m_grid->cols_count() + 1);
    } else {
        table_width_spacing = 0;

        if (m_grid->cols_count()) {
            table_width_spacing -=
                std::min(border_left(), m_grid->column(0).border_left);
            table_width_spacing -= std::min(border_right(),
                m_grid->column(m_grid->cols_count() - 1).border_right);
        }

        for (int col = 1; col < m_grid->cols_count(); col++) {
            table_width_spacing -= std::min(m_grid->column(col).border_left,
                m_grid->column(col - 1).border_right);
        }
    }


    // Calculate the minimum content width (MCW) of each cell: the formatted content
    // may span any number of lines but may not overflow the cell box. If the specified
    // 'width' (W) of the cell is greater than MCW, W is the minimum cell width. A value of 'auto' means that MCW is the minimum
    // cell width.
    //
    // Also, calculate the "maximum" cell width of each cell: formatting the content
    // without breaking lines other than where explicit line breaks occur.

    if (m_grid->cols_count() == 1 && !block_width.is_default()) {
        for (int row = 0; row < m_grid->rows_count(); row++) {
            table_cell* cell = m_grid->cell(0, row);
            if (cell && cell->el) {
                cell->min_width = cell->max_width =
                    cell->el->render(0, 0, max_width - table_width_spacing);
                cell->el->position_.width = cell->min_width -
                                        cell->el->content_margins_left() -
                                        cell->el->content_margins_right();
            }
        }
    } else {
        for (int row = 0; row < m_grid->rows_count(); row++) {
            for (int col = 0; col < m_grid->cols_count(); col++) {
                table_cell* cell = m_grid->cell(col, row);
                if (cell && cell->el) {
                    if (!m_grid->column(col).css_width.is_predefined() &&
                        m_grid->column(col).css_width.units() !=
                            kCSSUnitsPercent) {
                        int css_w = m_grid->column(col).css_width.calc_percent(
                            block_width);
                        int el_w = cell->el->render(0, 0, css_w);
                        cell->min_width = cell->max_width = std::max(css_w, el_w);
                        cell->el->position_.width =
                            cell->min_width - cell->el->content_margins_left() -
                            cell->el->content_margins_right();
                    } else {
                        // calculate minimum content width
                        cell->min_width = cell->el->render(0, 0, 1);
                        // calculate maximum content width
                        cell->max_width =
                            cell->el->render(0, 0, max_width - table_width_spacing);
                    }
                }
            }
        }
    }

    // For each column, determine a maximum and minimum column width from the
    // cells that span only that column. The minimum is that required by the
    // cell with the largest minimum cell width (or the column 'width',
    // whichever is larger). The maximum is that required by the cell with the
    // largest maximum cell width (or the column 'width', whichever is larger).

    for (int col = 0; col < m_grid->cols_count(); col++) {
        m_grid->column(col).max_width = 0;
        m_grid->column(col).min_width = 0;
        for (int row = 0; row < m_grid->rows_count(); row++) {
            if (m_grid->cell(col, row)->colspan <= 1) {
                m_grid->column(col).max_width =
                    std::max(m_grid->column(col).max_width,
                        m_grid->cell(col, row)->max_width);
                m_grid->column(col).min_width =
                    std::max(m_grid->column(col).min_width,
                        m_grid->cell(col, row)->min_width);
            }
        }
    }

    // For each cell that spans more than one column, increase the minimum
    // widths of the columns it spans so that together, they are at least as
    // wide as the cell. Do the same for the maximum widths. If possible, widen
    // all spanned columns by approximately the same amount.

    for (int col = 0; col < m_grid->cols_count(); col++) {
        for (int row = 0; row < m_grid->rows_count(); row++) {
            if (m_grid->cell(col, row)->colspan > 1) {
                int max_total_width = m_grid->column(col).max_width;
                int min_total_width = m_grid->column(col).min_width;
                for (int col2 = col + 1;
                     col2 < col + m_grid->cell(col, row)->colspan;
                     col2++) {
                    max_total_width += m_grid->column(col2).max_width;
                    min_total_width += m_grid->column(col2).min_width;
                }
                if (min_total_width < m_grid->cell(col, row)->min_width) {
                    m_grid->distribute_min_width(
                        m_grid->cell(col, row)->min_width - min_total_width,
                        col,
                        col + m_grid->cell(col, row)->colspan - 1);
                }
                if (max_total_width < m_grid->cell(col, row)->max_width) {
                    m_grid->distribute_max_width(
                        m_grid->cell(col, row)->max_width - max_total_width,
                        col,
                        col + m_grid->cell(col, row)->colspan - 1);
                }
            }
        }
    }

    // If the 'table' or 'inline-table' element's 'width' property has a
    // computed value (W) other than 'auto', the used width is the greater of W,
    // CAPMIN, and the minimum width required by all the columns plus cell
    // spacing or borders (MIN). If the used width is greater than MIN, the
    // extra width should be distributed over the columns.
    //
    // If the 'table' or 'inline-table' element has 'width: auto', the used
    // width is the greater of the table's containing block width, CAPMIN, and
    // MIN. However, if either CAPMIN or the maximum width required by the
    // columns plus cell spacing or borders (MAX) is less than that of the
    // containing block, use max(MAX, CAPMIN).


    int table_width = 0;
    int min_table_width = 0;
    int max_table_width = 0;

    if (!block_width.is_default()) {
        table_width = m_grid->calc_table_width(block_width - table_width_spacing,
            false,
            min_table_width,
            max_table_width);
    } else {
        table_width = m_grid->calc_table_width(max_width - table_width_spacing,
            true,
            min_table_width,
            max_table_width);
    }

    min_table_width += table_width_spacing;
    max_table_width += table_width_spacing;
    table_width += table_width_spacing;
    m_grid->calc_horizontal_positions(m_borders,
        m_border_collapse,
        m_border_spacing_x);

    bool row_span_found = false;

    // render cells with computed width
    for (int row = 0; row < m_grid->rows_count(); row++) {
        m_grid->row(row).height = 0;
        for (int col = 0; col < m_grid->cols_count(); col++) {
            table_cell* cell = m_grid->cell(col, row);
            if (cell->el) {
                int span_col = col + cell->colspan - 1;
                if (span_col >= m_grid->cols_count()) {
                    span_col = m_grid->cols_count() - 1;
                }
                int cell_width =
                    m_grid->column(span_col).right - m_grid->column(col).left;

                if (cell->el->position_.width !=
                    cell_width - cell->el->content_margins_left() -
                        cell->el->content_margins_right()) {
                    cell->el->render(m_grid->column(col).left, 0, cell_width);
                    cell->el->position_.width = cell_width -
                                            cell->el->content_margins_left() -
                                            cell->el->content_margins_right();
                } else {
                    cell->el->position_.x = m_grid->column(col).left +
                                        cell->el->content_margins_left();
                }

                if (cell->rowspan <= 1) {
                    m_grid->row(row).height =
                        std::max(m_grid->row(row).height, cell->el->height());
                } else {
                    row_span_found = true;
                }
            }
        }
    }

    if (row_span_found) {
        for (int col = 0; col < m_grid->cols_count(); col++) {
            for (int row = 0; row < m_grid->rows_count(); row++) {
                table_cell* cell = m_grid->cell(col, row);
                if (cell->el) {
                    int span_row = row + cell->rowspan - 1;
                    if (span_row >= m_grid->rows_count()) {
                        span_row = m_grid->rows_count() - 1;
                    }
                    if (span_row != row) {
                        int h = 0;
                        for (int i = row; i <= span_row; i++) {
                            h += m_grid->row(i).height;
                        }
                        if (h < cell->el->height()) {
                            m_grid->row(span_row).height += cell->el->height() - h;
                        }
                    }
                }
            }
        }
    }

    // Calculate vertical table spacing
    int table_height_spacing = 0;
    if (m_border_collapse == border_collapse_separate) {
        table_height_spacing = m_border_spacing_y * (m_grid->rows_count() + 1);
    } else {
        table_height_spacing = 0;

        if (m_grid->rows_count()) {
            table_height_spacing -=
                std::min(border_top(), m_grid->row(0).border_top);
            table_height_spacing -= std::min(border_bottom(),
                m_grid->row(m_grid->rows_count() - 1).border_bottom);
        }

        for (int row = 1; row < m_grid->rows_count(); row++) {
            table_height_spacing -= std::min(m_grid->row(row).border_top,
                m_grid->row(row - 1).border_bottom);
        }
    }


    // calculate block height
    int block_height = 0;
    if (get_predefined_height(block_height)) {
        block_height -= m_padding.height() + m_borders.height();
    }

    // calculate minimum height from m_css_min_height
    int min_height = 0;
    if (!m_css_min_height.is_predefined() &&
        m_css_min_height.units() == kCSSUnitsPercent) {
        Element::ptr el_parent = parent();
        if (el_parent) {
            int parent_height = 0;
            if (el_parent->get_predefined_height(parent_height)) {
                min_height = m_css_min_height.calc_percent(parent_height);
            }
        }
    } else {
        min_height = (int)m_css_min_height.val();
    }

    int minimum_table_height = std::max(block_height, min_height);

    m_grid->calc_rows_height(minimum_table_height - table_height_spacing,
        m_border_spacing_y);
    m_grid->calc_vertical_positions(m_borders, m_border_collapse, m_border_spacing_y);

    int table_height = 0;

    // place cells vertically
    for (int col = 0; col < m_grid->cols_count(); col++) {
        for (int row = 0; row < m_grid->rows_count(); row++) {
            table_cell* cell = m_grid->cell(col, row);
            if (cell->el) {
                int span_row = row + cell->rowspan - 1;
                if (span_row >= m_grid->rows_count()) {
                    span_row = m_grid->rows_count() - 1;
                }
                cell->el->position_.y =
                    m_grid->row(row).top + cell->el->content_margins_top();
                cell->el->position_.height = m_grid->row(span_row).bottom -
                                         m_grid->row(row).top -
                                         cell->el->content_margins_top() -
                                         cell->el->content_margins_bottom();
                table_height =
                    std::max(table_height, m_grid->row(span_row).bottom);
                cell->el->apply_vertical_align();
            }
        }
    }

    if (m_border_collapse == border_collapse_collapse) {
        if (m_grid->rows_count()) {
            table_height -= std::min(border_bottom(),
                m_grid->row(m_grid->rows_count() - 1).border_bottom);
        }
    } else {
        table_height += m_border_spacing_y;
    }

    position_.width = table_width;

    calc_auto_margins(parent_width);

    position_.move_to(x, y);
    position_.x += content_margins_left();
    position_.y += content_margins_top();
    position_.width = table_width;
    position_.height = table_height;

    return max_table_width;
}

void HTMLElement::draw_children_box(uintptr_t hdc,
    int x,
    int y,
    const Position* clip,
    DrawFlag flag,
    int zindex)
{
    Position pos = position_;
    pos.x += x;
    pos.y += y;

    Document* doc = get_document();

    if (overflow_ > kOverflowVisible) {
        Position border_box = pos;
        border_box += m_padding;
        border_box += m_borders;

        BorderRadii border_radii = m_css_borders.radii.calculate_radii(border_box.width, border_box.height);

        border_radii -= m_borders;
        border_radii -= m_padding;

        doc->container()->set_clip(pos, border_radii, true, true);
    }

    Position browser_wnd;
    doc->container()->get_client_rect(browser_wnd);

    Element::ptr el;
    for (auto& item : m_children) {
        el = item;
        if (el->is_visible()) {
            switch (flag) {
                case kDrawPositioned:
                    if (el->is_positioned() && el->get_zindex() == zindex) {
                        if (el->get_element_position() == kPositionFixed) {
                            el->draw(hdc, browser_wnd.x, browser_wnd.y, clip);
                            el->draw_stacking_context(hdc,
                                browser_wnd.x,
                                browser_wnd.y,
                                clip,
                                true);
                        } else {
                            el->draw(hdc, pos.x, pos.y, clip);
                            el->draw_stacking_context(hdc, pos.x, pos.y, clip, true);
                        }
                        el = nullptr;
                    }
                    break;
                case kDrawBlock:
                    if (!el->is_inline_box() && el->get_float() == kFloatNone &&
                        !el->is_positioned()) {
                        el->draw(hdc, pos.x, pos.y, clip);
                    }
                    break;
                case kDrawFloats:
                    if (el->get_float() != kFloatNone && !el->is_positioned()) {
                        el->draw(hdc, pos.x, pos.y, clip);
                        el->draw_stacking_context(hdc, pos.x, pos.y, clip, false);
                        el = nullptr;
                    }
                    break;
                case kDrawInlines:
                    if (el->is_inline_box() && el->get_float() == kFloatNone &&
                        !el->is_positioned()) {
                        el->draw(hdc, pos.x, pos.y, clip);
                        if (el->get_display() == kDisplayInlineBlock) {
                            el->draw_stacking_context(hdc, pos.x, pos.y, clip, false);
                            el = nullptr;
                        }
                    }
                    break;
                default:
                    break;
            }

            if (el) {
                if (flag == kDrawPositioned) {
                    if (!el->is_positioned()) {
                        el->draw_children(hdc, pos.x, pos.y, clip, flag, zindex);
                    }
                } else {
                    if (el->get_float() == kFloatNone &&
                        el->get_display() != kDisplayInlineBlock &&
                        !el->is_positioned()) {
                        el->draw_children(hdc, pos.x, pos.y, clip, flag, zindex);
                    }
                }
            }
        }
    }

    if (overflow_ > kOverflowVisible) {
        doc->container()->del_clip();
    }
}

void HTMLElement::draw_children_table(uintptr_t hdc,
    int x,
    int y,
    const Position* clip,
    DrawFlag flag,
    int zindex)
{
    if (!m_grid)
        return;

    Position pos = position_;
    pos.x += x;
    pos.y += y;
    for (int row = 0; row < m_grid->rows_count(); row++) {
        if (flag == kDrawBlock) {
            m_grid->row(row).el_row->draw_background(hdc, pos.x, pos.y, clip);
        }
        for (int col = 0; col < m_grid->cols_count(); col++) {
            table_cell* cell = m_grid->cell(col, row);
            if (cell->el) {
                if (flag == kDrawBlock) {
                    cell->el->draw(hdc, pos.x, pos.y, clip);
                }
                cell->el->draw_children(hdc, pos.x, pos.y, clip, flag, zindex);
            }
        }
    }
}

} // namespace litehtml
