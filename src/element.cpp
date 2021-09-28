// Copyright (c) 2013, Yuri Kobets (tordex)
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

#include "litehtml/element.h"

#include <algorithm>

#include "litehtml/document.h"
#include "litehtml/document_container.h"

namespace litehtml {

element::element(const std::shared_ptr<document>& doc)
: m_doc(doc)
{
    m_box = 0;
    m_skip = false;
}

element::~element()
{
}

bool element::is_point_inside(int x, int y)
{
    if (get_display() != display_inline && get_display() != display_table_row) {
        position pos = m_pos;
        pos += m_padding;
        pos += m_borders;
        if (pos.is_point_inside(x, y)) {
            return true;
        } else {
            return false;
        }
    } else {
        position::vector boxes;
        get_inline_boxes(boxes);
        for (auto box = boxes.begin(); box != boxes.end(); box++) {
            if (box->is_point_inside(x, y)) {
                return true;
            }
        }
    }
    return false;
}

web_color element::get_color(const tchar_t* prop_name,
    bool inherited,
    const web_color& def_color)
{
    const tchar_t* clrstr = get_style_property(prop_name, inherited, 0);
    if (!clrstr) {
        return def_color;
    }
    return web_color::from_string(clrstr, get_document()->container());
}

position element::get_placement() const
{
    position pos = m_pos;
    element::ptr cur_el = parent();
    while (cur_el) {
        pos.x += cur_el->m_pos.x;
        pos.y += cur_el->m_pos.y;
        cur_el = cur_el->parent();
    }
    return pos;
}

bool element::is_inline_box() const
{
    style_display d = get_display();
    if (d == display_inline || d == display_inline_table ||
        d == display_inline_block || d == display_inline_text) {
        return true;
    }
    return false;
}

bool element::collapse_top_margin() const
{
    if (!m_borders.top && !m_padding.top && in_normal_flow() &&
        get_float() == float_none && m_margins.top >= 0 && have_parent()) {
        return true;
    }
    return false;
}

bool element::collapse_bottom_margin() const
{
    if (!m_borders.bottom && !m_padding.bottom && in_normal_flow() &&
        get_float() == float_none && m_margins.bottom >= 0 && have_parent()) {
        return true;
    }
    return false;
}

bool element::get_predefined_height(int& p_height) const
{
    css_length h = get_css_height();
    if (h.is_predefined()) {
        p_height = m_pos.height;
        return false;
    }
    if (h.units() == css_units_percentage) {
        element::ptr el_parent = parent();
        if (!el_parent) {
            position client_pos;
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
                p_height = m_pos.height;
                return false;
            }
        }
    }
    p_height = get_document()->cvt_units(h, get_font_size());
    return true;
}

void element::calc_document_size(size& sz, int x /*= 0*/, int y /*= 0*/)
{
    if (is_visible()) {
        sz.width = std::max(sz.width, x + right());
        sz.height = std::max(sz.height, y + bottom());
    }
}

void element::get_redraw_box(position& pos, int x /*= 0*/, int y /*= 0*/)
{
    if (is_visible()) {
        int p_left = std::min(pos.left(),
            x + m_pos.left() - m_padding.left - m_borders.left);
        int p_right = std::max(pos.right(),
            x + m_pos.right() + m_padding.left + m_borders.left);
        int p_top =
            std::min(pos.top(), y + m_pos.top() - m_padding.top - m_borders.top);
        int p_bottom = std::max(pos.bottom(),
            y + m_pos.bottom() + m_padding.bottom + m_borders.bottom);

        pos.x = p_left;
        pos.y = p_top;
        pos.width = p_right - p_left;
        pos.height = p_bottom - p_top;
    }
}

int element::calc_width(int defVal) const
{
    css_length w = get_css_width();
    if (w.is_predefined()) {
        return defVal;
    }
    if (w.units() == css_units_percentage) {
        element::ptr el_parent = parent();
        if (!el_parent) {
            position client_pos;
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

bool element::is_ancestor(const ptr& el) const
{
    element::ptr el_parent = parent();
    while (el_parent && el_parent != el) {
        el_parent = el_parent->parent();
    }
    if (el_parent) {
        return true;
    }
    return false;
}

int element::get_inline_shift_left()
{
    int ret = 0;
    element::ptr el_parent = parent();
    if (el_parent) {
        if (el_parent->get_display() == display_inline) {
            style_display disp = get_display();

            if (disp == display_inline_text || disp == display_inline_block) {
                element::ptr el = shared_from_this();
                while (el_parent && el_parent->get_display() == display_inline) {
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

int element::get_inline_shift_right()
{
    int ret = 0;
    element::ptr el_parent = parent();
    if (el_parent) {
        if (el_parent->get_display() == display_inline) {
            style_display disp = get_display();

            if (disp == display_inline_text || disp == display_inline_block) {
                element::ptr el = shared_from_this();
                while (el_parent && el_parent->get_display() == display_inline) {
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

void element::apply_relative_shift(int parent_width)
{
    css_offsets offsets;
    if (get_element_position(&offsets) == element_position_relative) {
        element::ptr parent_ptr = parent();
        if (!offsets.left.is_predefined()) {
            m_pos.x += offsets.left.calc_percent(parent_width);
        } else if (!offsets.right.is_predefined()) {
            m_pos.x -= offsets.right.calc_percent(parent_width);
        }
        if (!offsets.top.is_predefined()) {
            int h = 0;

            if (offsets.top.units() == css_units_percentage) {
                element::ptr el_parent = parent();
                if (el_parent) {
                    el_parent->get_predefined_height(h);
                }
            }

            m_pos.y += offsets.top.calc_percent(h);
        } else if (!offsets.bottom.is_predefined()) {
            int h = 0;

            if (offsets.top.units() == css_units_percentage) {
                element::ptr el_parent = parent();
                if (el_parent) {
                    el_parent->get_predefined_height(h);
                }
            }

            m_pos.y -= offsets.bottom.calc_percent(h);
        }
    }
}

void element::calc_auto_margins(int parent_width)
{
}

const background* element::get_background(bool own_only)
{
    return 0;
}

element::ptr element::get_element_by_point(int x, int y, int client_x, int client_y)
{
    return 0;
}

element::ptr element::get_child_by_point(int x,
    int y,
    int client_x,
    int client_y,
    draw_flag flag,
    int zindex)
{
    return 0;
}

void element::get_line_left_right(int y, int def_right, int& ln_left, int& ln_right)
{
}

void element::add_style(const style& st)
{
}

void element::select_all(const css_selector& selector, elements_vector& res)
{
}

elements_vector element::select_all(const css_selector& selector)
{
    return elements_vector();
}

elements_vector element::select_all(const tstring& selector)
{
    return elements_vector();
}

element::ptr element::select_one(const css_selector& selector)
{
    return 0;
}

element::ptr element::select_one(const tstring& selector)
{
    return 0;
}

element::ptr element::find_adjacent_sibling(const element::ptr& el,
    const css_selector& selector,
    bool apply_pseudo /*= true*/,
    bool* is_pseudo /*= 0*/)
{
    return 0;
}

element::ptr element::find_sibling(const element::ptr& el,
    const css_selector& selector,
    bool apply_pseudo /*= true*/,
    bool* is_pseudo /*= 0*/)
{
    return 0;
}

bool element::is_nth_last_child(const element::ptr& el, int num, int off, bool of_type) const
{
    return false;
}

bool element::is_nth_child(const element::ptr&, int num, int off, bool of_type) const
{
    return false;
}

bool element::is_only_child(const element::ptr& el, bool of_type) const
{
    return false;
}

overflow element::get_overflow() const
{
    return overflow_visible;
}

void element::draw_children(uint_ptr hdc,
    int x,
    int y,
    const position* clip,
    draw_flag flag,
    int zindex)
{
}

void element::draw_stacking_context(uint_ptr hdc,
    int x,
    int y,
    const position* clip,
    bool with_positioned)
{
}

void element::render_positioned(render_type rt)
{
}

int element::get_zindex() const
{
    return 0;
}

bool element::fetch_positioned()
{
    return false;
}

visibility element::get_visibility() const
{
    return visibility_visible;
}

void element::apply_vertical_align()
{
}

void element::set_css_width(css_length& w)
{
}

element::ptr element::get_child(int idx) const
{
    return 0;
}

size_t element::get_children_count() const
{
    return 0;
}

void element::calc_outlines(int parent_width)
{
}

css_length element::get_css_width() const
{
    return css_length();
}

css_length element::get_css_height() const
{
    return css_length();
}

element_clear element::get_clear() const
{
    return clear_none;
}

css_length element::get_css_left() const
{
    return css_length();
}

css_length element::get_css_right() const
{
    return css_length();
}

css_length element::get_css_top() const
{
    return css_length();
}

css_length element::get_css_bottom() const
{
    return css_length();
}

css_offsets element::get_css_offsets() const
{
    return css_offsets();
}

vertical_align element::get_vertical_align() const
{
    return va_baseline;
}

int element::place_element(const ptr& el, int max_width)
{
    return 0;
}

int element::render_inline(const ptr& container, int max_width)
{
    return 0;
}

void element::add_positioned(const ptr& el)
{
}

int element::find_next_line_top(int top, int width, int def_right)
{
    return 0;
}

element_float element::get_float() const
{
    return float_none;
}

void element::add_float(const ptr& el, int x, int y)
{
}

void element::update_floats(int dy, const ptr& parent)
{
}

int element::get_line_left(int y)
{
    return 0;
}

int element::get_line_right(int y, int def_right)
{
    return def_right;
}

int element::get_left_floats_height() const
{
    return 0;
}

int element::get_right_floats_height() const
{
    return 0;
}

int element::get_floats_height(element_float el_float) const
{
    return 0;
}

bool element::is_floats_holder() const
{
    return false;
}

void element::get_content_size(size& sz, int max_width)
{
}

void element::init()
{
}

int element::render(int x, int y, int max_width, bool second_pass)
{
    return 0;
}

bool element::appendChild(const ptr& el)
{
    return false;
}

bool element::removeChild(const ptr& el)
{
    return false;
}

void element::clearRecursive()
{
}

const tchar_t* element::get_tagName() const
{
    return _t("");
}

void element::set_tagName(const tchar_t* tag)
{
}

void element::set_data(const tchar_t* data)
{
}

void element::set_attr(const tchar_t* name, const tchar_t* val)
{
}

void element::apply_stylesheet(const css_stylesheet& stylesheet)
{
}

void element::refresh_styles()
{
}

void element::on_click()
{
}

void element::init_font()
{
}

void element::get_inline_boxes(position::vector& boxes)
{
}

void element::parse_styles(bool is_reparse /*= false*/)
{
}

const tchar_t* element::get_attr(const tchar_t* name,
    const tchar_t* def /*= 0*/) const
{
    return def;
}

bool element::is_white_space() const
{
    return false;
}

bool element::is_body() const
{
    return false;
}

bool element::is_break() const
{
    return false;
}

int element::get_base_line()
{
    return 0;
}

bool element::on_mouse_over()
{
    return false;
}

bool element::on_mouse_leave()
{
    return false;
}

bool element::on_lbutton_down()
{
    return false;
}

bool element::on_lbutton_up()
{
    return false;
}

bool element::find_styles_changes(position::vector& redraw_boxes, int x, int y)
{
    return false;
}

const tchar_t* element::get_cursor()
{
    return 0;
}

white_space element::get_white_space() const
{
    return white_space_normal;
}

style_display element::get_display() const
{
    return display_none;
}

bool element::set_pseudo_class(const tchar_t* pclass, bool add)
{
    return false;
}

bool element::set_class(const tchar_t* pclass, bool add)
{
    return false;
}

element_position element::get_element_position(css_offsets* offsets) const
{
    return element_position_static;
}

bool element::is_replaced() const
{
    return false;
}

int element::line_height() const
{
    return 0;
}

void element::draw(uint_ptr hdc, int x, int y, const position* clip)
{
}

void element::draw_background(uint_ptr hdc, int x, int y, const position* clip)
{
}

const tchar_t* element::get_style_property(const tchar_t* name,
    bool inherited,
    const tchar_t* def /*= 0*/)
{
    return 0;
}

uint_ptr element::get_font(font_metrics* fm /*= 0*/)
{
    return 0;
}

int element::get_font_size() const
{
    return 0;
}

void element::get_text(tstring& text)
{
}

void element::parse_attributes()
{
}

int element::select(const css_selector& selector, bool apply_pseudo)
{
    return select_no_match;
}

int element::select(const css_element_selector& selector,
    bool apply_pseudo /*= true*/)
{
    return select_no_match;
}

element::ptr element::find_ancestor(const css_selector& selector,
    bool apply_pseudo,
    bool* is_pseudo)
{
    return 0;
}

bool element::is_first_child_inline(const element::ptr& el) const
{
    return false;
}

bool element::is_last_child_inline(const element::ptr& el)
{
    return false;
}

bool element::have_inline_child() const
{
    return false;
}

} // namespace litehtml
