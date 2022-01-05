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

#include "litehtml/element/element.h"

#include <algorithm>

#include "litehtml/document.h"
#include "litehtml/document_container.h"
#include "litehtml/logging.h"

namespace litehtml {

Element::Element(Document* document)
: m_doc(document)
, m_parent(nullptr)
, m_box(nullptr)
, m_skip(false)
{
}

Element::~Element()
{
    for (auto child : m_children) {
        delete child;
    }
}

bool Element::is_point_inside(int x, int y)
{
    if (get_display() != kDisplayInline && get_display() != kDisplayTableRow) {
        Position pos = position_;
        pos += m_padding;
        pos += m_borders;
        if (pos.is_point_inside(x, y)) {
            return true;
        } else {
            return false;
        }
    } else {
        std::vector<Position> boxes;
        get_inline_boxes(boxes);
        for (auto box = boxes.begin(); box != boxes.end(); box++) {
            if (box->is_point_inside(x, y)) {
                return true;
            }
        }
    }
    return false;
}

WebColor Element::get_color(CSSProperty name, const WebColor& default_color)
{
    const CSSValue* value = get_style_property_value(name);
    if (!value) {
        // FIXME: Load the appropriate default value.
        return default_color;
    }
    assert(value->is_color());
    return static_cast<const CSSColorValue*>(value)->color();
}

CSSLength Element::get_length(CSSProperty property) const
{
    const CSSValue* value = get_style_property_value(property);
    if (!value) {
        // FIXME: Load the appropriate default value.
        return CSSLength();
    }
    assert(value->is_length());
    return static_cast<const CSSLengthValue*>(value)->length();
}

Position Element::get_placement() const
{
    Position pos = position_;
    Element::ptr cur_el = parent();
    while (cur_el) {
        pos.x += cur_el->position_.x;
        pos.y += cur_el->position_.y;
        cur_el = cur_el->parent();
    }
    return pos;
}

bool Element::is_inline_box() const
{
    Display d = get_display();
    if (d == kDisplayInline || d == kDisplayInlineTable ||
        d == kDisplayInlineBlock || d == kDisplayInlineText) {
        return true;
    }
    return false;
}

bool Element::collapse_top_margin() const
{
    if (!m_borders.top && !m_padding.top && in_normal_flow() &&
        get_float() == kFloatNone && m_margins.top >= 0 && have_parent()) {
        return true;
    }
    return false;
}

bool Element::collapse_bottom_margin() const
{
    if (!m_borders.bottom && !m_padding.bottom && in_normal_flow() &&
        get_float() == kFloatNone && m_margins.bottom >= 0 && have_parent()) {
        return true;
    }
    return false;
}

bool Element::get_predefined_height(int& p_height) const
{
    CSSLength h = get_css_height();
    if (h.is_predefined()) {
        p_height = position_.height;
        return false;
    }
    if (h.units() == kCSSUnitsPercent) {
        Element::ptr el_parent = parent();
        if (!el_parent) {
            Position client_pos;
            get_document()->container()->get_client_rect(client_pos);
            p_height = h.calc_percent(client_pos.height);
            return true;
        } else {
            int ph = 0;
            if (el_parent->get_predefined_height(ph)) {
                p_height = h.calc_percent(ph);
                if (is_body()) {
                    p_height -= content_margins_height();
                }
                return true;
            } else {
                p_height = position_.height;
                return false;
            }
        }
    }
    p_height = get_document()->cvt_units(h, get_font_size());
    return true;
}

void Element::calc_document_size(Size& sz, int x /*= 0*/, int y /*= 0*/)
{
    if (is_visible()) {
        sz.width = std::max(sz.width, x + right());
        sz.height = std::max(sz.height, y + bottom());
    }
}

void Element::get_redraw_box(Position& pos, int x /*= 0*/, int y /*= 0*/)
{
    if (is_visible()) {
        int p_left = std::min(pos.left(),
            x + position_.left() - m_padding.left - m_borders.left);
        int p_right = std::max(pos.right(),
            x + position_.right() + m_padding.left + m_borders.left);
        int p_top =
            std::min(pos.top(), y + position_.top() - m_padding.top - m_borders.top);
        int p_bottom = std::max(pos.bottom(),
            y + position_.bottom() + m_padding.bottom + m_borders.bottom);

        pos.x = p_left;
        pos.y = p_top;
        pos.width = p_right - p_left;
        pos.height = p_bottom - p_top;
    }
}

int Element::calc_width(int defVal) const
{
    CSSLength w = get_css_width();
    if (w.is_predefined()) {
        return defVal;
    }
    if (w.units() == kCSSUnitsPercent) {
        Element::ptr el_parent = parent();
        if (!el_parent) {
            Position client_pos;
            get_document()->container()->get_client_rect(client_pos);
            return w.calc_percent(client_pos.width);
        } else {
            int pw = el_parent->calc_width(defVal);
            if (is_body()) {
                pw -= content_margins_width();
            }
            return w.calc_percent(pw);
        }
    }
    return get_document()->cvt_units(w, get_font_size());
}

bool Element::is_ancestor(const ptr& el) const
{
    Element::ptr el_parent = parent();
    while (el_parent && el_parent != el) {
        el_parent = el_parent->parent();
    }
    if (el_parent) {
        return true;
    }
    return false;
}

int Element::get_inline_shift_left()
{
    int ret = 0;
    Element::ptr el_parent = parent();
    if (el_parent) {
        if (el_parent->get_display() == kDisplayInline) {
            Display disp = get_display();

            if (disp == kDisplayInlineText || disp == kDisplayInlineBlock) {
                Element* el = this;
                while (el_parent && el_parent->get_display() == kDisplayInline) {
                    if (el_parent->is_first_child_inline(el)) {
                        ret += el_parent->padding_left() +
                               el_parent->border_left() + el_parent->margin_left();
                    }
                    el = el_parent;
                    el_parent = el_parent->parent();
                }
            }
        }
    }

    return ret;
}

int Element::get_inline_shift_right()
{
    int ret = 0;
    Element::ptr el_parent = parent();
    if (el_parent) {
        if (el_parent->get_display() == kDisplayInline) {
            Display disp = get_display();

            if (disp == kDisplayInlineText || disp == kDisplayInlineBlock) {
                Element* el = this;
                while (el_parent && el_parent->get_display() == kDisplayInline) {
                    if (el_parent->is_last_child_inline(el)) {
                        ret += el_parent->padding_right() +
                               el_parent->border_right() +
                               el_parent->margin_right();
                    }
                    el = el_parent;
                    el_parent = el_parent->parent();
                }
            }
        }
    }

    return ret;
}

void Element::apply_relative_shift(int parent_width)
{
    CSSOffsets offsets;
    if (get_element_position(&offsets) == kPositionRelative) {
        if (!offsets.left.is_predefined()) {
            position_.x += offsets.left.calc_percent(parent_width);
        } else if (!offsets.right.is_predefined()) {
            position_.x -= offsets.right.calc_percent(parent_width);
        }
        if (!offsets.top.is_predefined()) {
            int h = 0;

            if (offsets.top.units() == kCSSUnitsPercent) {
                Element::ptr el_parent = parent();
                if (el_parent) {
                    el_parent->get_predefined_height(h);
                }
            }

            position_.y += offsets.top.calc_percent(h);
        } else if (!offsets.bottom.is_predefined()) {
            int h = 0;

            if (offsets.top.units() == kCSSUnitsPercent) {
                Element::ptr el_parent = parent();
                if (el_parent) {
                    el_parent->get_predefined_height(h);
                }
            }

            position_.y -= offsets.bottom.calc_percent(h);
        }
    }
}

void Element::calc_auto_margins(int)
{
}

const Background* Element::get_background(bool)
{
    return nullptr;
}

Element::ptr Element::get_element_by_point(int, int, int, int )
{
    return nullptr;
}

Element::ptr Element::get_child_by_point(int,
    int,
    int,
    int,
    DrawFlag,
    int)
{
    return nullptr;
}

void Element::get_line_left_right(int, int, int&, int&)
{
}

void Element::add_style(const CSSStyle&)
{
}

void Element::select_all(const CSSSelector&, ElementsVector&)
{
}

ElementsVector Element::select_all(const CSSSelector&)
{
    return ElementsVector();
}

ElementsVector Element::select_all(const tstring&)
{
    return ElementsVector();
}

Element::ptr Element::select_one(const CSSSelector&)
{
    return nullptr;
}

Element::ptr Element::select_one(const tstring&)
{
    return nullptr;
}

Element::ptr Element::find_adjacent_sibling(const Element::ptr&,
    const CSSSelector&,
    bool,
    bool*)
{
    return nullptr;
}

Element::ptr Element::find_sibling(const Element::ptr&,
    const CSSSelector&,
    bool,
    bool*)
{
    return nullptr;
}

bool Element::is_nth_last_child(const Element::ptr&, int, int, bool) const
{
    return false;
}

bool Element::is_nth_child(const Element::ptr&, int, int, bool) const
{
    return false;
}

bool Element::is_only_child(const Element::ptr&, bool) const
{
    return false;
}

Overflow Element::get_overflow() const
{
    return kOverflowVisible;
}

void Element::draw_children(uintptr_t,
    int,
    int,
    const Position*,
    DrawFlag,
    int)
{
}

void Element::draw_stacking_context(uintptr_t,
    int,
    int,
    const Position*,
    bool)
{
}

void Element::render_positioned(RenderType)
{
}

int Element::get_zindex() const
{
    return 0;
}

bool Element::fetch_positioned()
{
    return false;
}

Visibility Element::get_visibility() const
{
    return kVisibilityVisible;
}

void Element::apply_vertical_align()
{
}

void Element::set_css_width(CSSLength&)
{
}

Element::ptr Element::get_child(int) const
{
    return nullptr;
}

size_t Element::get_children_count() const
{
    return 0;
}

void Element::calc_outlines(int)
{
}

CSSLength Element::get_css_width() const
{
    return CSSLength();
}

CSSLength Element::get_css_height() const
{
    return CSSLength();
}

ElementClear Element::get_clear() const
{
    return kClearNone;
}

CSSLength Element::get_css_left() const
{
    return CSSLength();
}

CSSLength Element::get_css_right() const
{
    return CSSLength();
}

CSSLength Element::get_css_top() const
{
    return CSSLength();
}

CSSLength Element::get_css_bottom() const
{
    return CSSLength();
}

CSSOffsets Element::get_css_offsets() const
{
    return CSSOffsets();
}

VerticalAlign Element::get_vertical_align() const
{
    return kVerticalAlignBaseline;
}

int Element::place_element(const ptr&, int)
{
    return 0;
}

int Element::render_inline(const ptr&, int)
{
    return 0;
}

void Element::add_positioned(const ptr&)
{
}

int Element::find_next_line_top(int, int, int)
{
    return 0;
}

ElementFloat Element::get_float() const
{
    return kFloatNone;
}

void Element::add_float(const ptr&, int, int)
{
}

void Element::update_floats(int, const ptr&)
{
}

int Element::get_line_left(int)
{
    return 0;
}

int Element::get_line_right(int, int def_right)
{
    return def_right;
}

int Element::get_left_floats_height() const
{
    return 0;
}

int Element::get_right_floats_height() const
{
    return 0;
}

int Element::get_floats_height(ElementFloat) const
{
    return 0;
}

bool Element::is_floats_holder() const
{
    return false;
}

void Element::get_content_size(Size&, int)
{
}

void Element::init()
{
}

int Element::render(int, int, int, bool)
{
    return 0;
}

bool Element::append_child(Element*)
{
    return false;
}

void Element::append_children(ElementsVector& children)
{
    for (Element* child : children) {
        if (!append_child(child)) {
            delete child;
        }
    }
}

void Element::clearRecursive()
{
}

const tchar_t* Element::get_tagName() const
{
    return _t("");
}

void Element::set_tagName(const tchar_t*)
{
}

void Element::set_data(const tchar_t*)
{
}

void Element::set_attr(const tchar_t*, const tchar_t*)
{
}

void Element::apply_stylesheet(const CSSStylesheet&)
{
}

void Element::refresh_styles()
{
}

void Element::on_click()
{
}

void Element::init_font()
{
}

void Element::get_inline_boxes(std::vector<Position>&)
{
}

void Element::parse_styles(bool)
{
}

const tchar_t* Element::get_attr(const tchar_t*,
    const tchar_t* def /*= 0*/) const
{
    return def;
}

bool Element::is_white_space() const
{
    return false;
}

bool Element::is_body() const
{
    return false;
}

bool Element::is_break() const
{
    return false;
}

int Element::get_baseline()
{
    return 0;
}

bool Element::on_mouse_over()
{
    return false;
}

bool Element::on_mouse_leave()
{
    return false;
}

bool Element::on_lbutton_down()
{
    return false;
}

bool Element::on_lbutton_up()
{
    return false;
}

bool Element::find_styles_changes(std::vector<Position>&, int, int)
{
    return false;
}

const tchar_t* Element::get_cursor()
{
    return nullptr;
}

WhiteSpace Element::get_white_space() const
{
    return kWhiteSpaceNormal;
}

Display Element::get_display() const
{
    return kDisplayNone;
}

bool Element::set_pseudo_class(const tchar_t*, bool)
{
    return false;
}

bool Element::set_class(const tchar_t*, bool)
{
    return false;
}

ElementPosition Element::get_element_position(CSSOffsets*) const
{
    return kPositionStatic;
}

bool Element::is_replaced() const
{
    return false;
}

int Element::line_height() const
{
    return 0;
}

void Element::draw(uintptr_t, int, int, const Position*)
{
}

void Element::draw_background(uintptr_t, int, int, const Position*)
{
}

const tchar_t* Element::get_style_property(CSSProperty)
{
    return nullptr;
}

const CSSValue* Element::get_style_property_value(CSSProperty) const
{
    return nullptr;
}

uintptr_t Element::get_font(FontMetrics*)
{
    return 0;
}

int Element::get_font_size() const
{
    return 0;
}

void Element::get_text(tstring&) const
{
}

void Element::parse_attributes()
{
}

int Element::select(const CSSSelector&, bool)
{
    return select_no_match;
}

int Element::select(const css_element_selector&,
    bool)
{
    return select_no_match;
}

Element::ptr Element::find_ancestor(const CSSSelector&,
    bool,
    bool*)
{
    return nullptr;
}

bool Element::is_first_child_inline(const Element::ptr&) const
{
    return false;
}

bool Element::is_last_child_inline(const Element::ptr&)
{
    return false;
}

bool Element::have_inline_child() const
{
    return false;
}

} // namespace litehtml
