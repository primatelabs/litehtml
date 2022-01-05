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

#ifndef LITEHTML_HTML_ELEMENT_H__
#define LITEHTML_HTML_ELEMENT_H__

#include "litehtml/background.h"
#include "litehtml/background_paint.h"
#include "litehtml/borders.h"
#include "litehtml/box.h"
#include "litehtml/css/css_margins.h"
#include "litehtml/css/css_selector.h"
#include "litehtml/css/css_stylesheet.h"
#include "litehtml/element/element.h"
#include "litehtml/css/css_style.h"
#include "litehtml/table.h"

namespace litehtml {

struct line_context {
    int calculatedTop;
    int top;
    int left;
    int right;

    int width()
    {
        return right - left;
    }
    void fix_top()
    {
        calculatedTop = top;
    }
};

class HTMLElement : public Element {
    friend class elements_iterator;
    friend class TableElement;
    friend class table_grid;
    friend class BlockBox;
    friend class line_box;

public:
    typedef std::shared_ptr<litehtml::HTMLElement> ptr;

protected:
    Box::vector m_boxes;
    string_vector m_class_values;
    tstring m_tag;
    CSSStyle m_style;
    string_map m_attrs;
    VerticalAlign vertical_align_;
    TextAlign m_text_align;
    Display m_display;
    ListStyleType list_style_type_;
    ListStylePosition list_style_position_;
    WhiteSpace white_space_;
    ElementFloat m_float;
    ElementClear m_clear;
    floated_box::vector m_floats_left;
    floated_box::vector m_floats_right;
    ElementsVector m_positioned;
    Background m_bg;
    ElementPosition m_el_position;
    int line_height_;
    bool m_lh_predefined;
    string_vector m_pseudo_classes;
    used_selector::vector m_used_styles;

    uintptr_t font_;
    int font_size_;
    FontMetrics font_metrics_;

    CSSMargins m_css_margins;
    CSSMargins m_css_padding;
    css_borders m_css_borders;
    CSSLength m_css_width;
    CSSLength m_css_height;
    CSSLength m_css_min_width;
    CSSLength m_css_min_height;
    CSSLength m_css_max_width;
    CSSLength m_css_max_height;
    CSSOffsets m_css_offsets;
    CSSLength m_css_text_indent;

    Overflow overflow_;
    Visibility m_visibility;
    int m_z_index;
    BoxSizing box_sizing_;

    int_int_cache m_cahe_line_left;
    int_int_cache m_cahe_line_right;

    // data for table rendering
    std::unique_ptr<table_grid> m_grid;
    CSSLength m_css_border_spacing_x;
    CSSLength m_css_border_spacing_y;
    int m_border_spacing_x;
    int m_border_spacing_y;
    border_collapse m_border_collapse;

    virtual void select_all(const CSSSelector& selector,
        ElementsVector& res) override;

public:
    HTMLElement(Document* doc);
    virtual ~HTMLElement() override;

    /* render functions */

    virtual int render(int x, int y, int max_width, bool second_pass = false) override;

    virtual int render_inline(const Element::ptr& container, int max_width) override;
    virtual int place_element(const Element::ptr& el, int max_width) override;
    virtual bool fetch_positioned() override;
    virtual void render_positioned(RenderType rt = kRenderAll) override;

    int new_box(const Element::ptr& el, int max_width, line_context& line_ctx);

    int get_cleared_top(const Element::ptr& el, int line_top) const;
    int finish_last_box(bool end_of_render = false);

    virtual bool appendChild(const Element::ptr& el) override;
    virtual void clearRecursive() override;
    virtual const tchar_t* get_tagName() const override;
    virtual void set_tagName(const tchar_t* tag) override;
    virtual void set_data(const tchar_t* data) override;
    virtual ElementFloat get_float() const override;
    virtual VerticalAlign get_vertical_align() const override;
    virtual CSSLength get_css_left() const override;
    virtual CSSLength get_css_right() const override;
    virtual CSSLength get_css_top() const override;
    virtual CSSLength get_css_bottom() const override;
    virtual CSSLength get_css_width() const override;
    virtual CSSOffsets get_css_offsets() const override;
    virtual void set_css_width(CSSLength& w) override;
    virtual CSSLength get_css_height() const override;
    virtual ElementClear get_clear() const override;
    virtual size_t get_children_count() const override;
    virtual Element::ptr get_child(int idx) const override;
    virtual ElementPosition get_element_position(
        CSSOffsets* offsets = nullptr) const override;
    virtual Overflow get_overflow() const override;

    virtual void set_attr(const tchar_t* name, const tchar_t* val) override;
    virtual const tchar_t* get_attr(const tchar_t* name,
        const tchar_t* def = nullptr) const override;
    virtual void apply_stylesheet(
        const litehtml::CSSStylesheet& stylesheet) override;
    virtual void refresh_styles() override;

    virtual bool is_white_space() const override;
    virtual bool is_body() const override;
    virtual bool is_break() const override;
    virtual int get_baseline() override;
    virtual bool on_mouse_over() override;
    virtual bool on_mouse_leave() override;
    virtual bool on_lbutton_down() override;
    virtual bool on_lbutton_up() override;
    virtual void on_click() override;
    virtual bool find_styles_changes(std::vector<Position>& redraw_boxes,
        int x,
        int y) override;
    virtual const tchar_t* get_cursor() override;
    virtual void init_font() override;
    virtual bool set_pseudo_class(const tchar_t* pclass, bool add) override;
    virtual bool set_class(const tchar_t* pclass, bool add) override;
    virtual bool is_replaced() const override;
    virtual int line_height() const override;
    virtual WhiteSpace get_white_space() const override;
    virtual Display get_display() const override;
    virtual Visibility get_visibility() const override;
    virtual void parse_styles(bool is_reparse = false) override;
    virtual void draw(uintptr_t hdc, int x, int y, const Position* clip) override;
    virtual void draw_background(uintptr_t hdc, int x, int y, const Position* clip) override;

    virtual const tchar_t* get_style_property(CSSProperty name) override;

    virtual const CSSValue* get_style_property_value(CSSProperty property) const override;

    virtual uintptr_t get_font(FontMetrics* fm = nullptr) override;
    virtual int get_font_size() const override;

    virtual void calc_outlines(int parent_width) override;
    virtual void calc_auto_margins(int parent_width) override;

    virtual int select(const CSSSelector& selector,
        bool apply_pseudo = true) override;
    virtual int select(const css_element_selector& selector,
        bool apply_pseudo = true) override;

    virtual ElementsVector select_all(const tstring& selector) override;
    virtual ElementsVector select_all(const CSSSelector& selector) override;

    virtual Element::ptr select_one(const tstring& selector) override;
    virtual Element::ptr select_one(const CSSSelector& selector) override;

    virtual Element::ptr find_ancestor(const CSSSelector& selector,
        bool apply_pseudo = true,
        bool* is_pseudo = nullptr) override;
    virtual Element::ptr find_adjacent_sibling(const Element::ptr& el,
        const CSSSelector& selector,
        bool apply_pseudo = true,
        bool* is_pseudo = nullptr) override;
    virtual Element::ptr find_sibling(const Element::ptr& el,
        const CSSSelector& selector,
        bool apply_pseudo = true,
        bool* is_pseudo = nullptr) override;
    virtual void get_text(tstring& text) override;
    virtual void parse_attributes() override;

    virtual bool is_first_child_inline(const Element::ptr& el) const override;
    virtual bool is_last_child_inline(const Element::ptr& el) override;
    virtual bool have_inline_child() const override;
    virtual void get_content_size(Size& sz, int max_width) override;
    virtual void init() override;
    virtual void get_inline_boxes(std::vector<Position>& boxes) override;
    virtual bool is_floats_holder() const override;
    virtual int get_floats_height(ElementFloat el_float = kFloatNone) const override;
    virtual int get_left_floats_height() const override;
    virtual int get_right_floats_height() const override;
    virtual int get_line_left(int y) override;
    virtual int get_line_right(int y, int def_right) override;
    virtual void get_line_left_right(int y,
        int def_right,
        int& ln_left,
        int& ln_right) override;
    virtual void add_float(const Element::ptr& el, int x, int y) override;
    virtual void update_floats(int dy, const Element::ptr& parent) override;
    virtual void add_positioned(const Element::ptr& el) override;
    virtual int find_next_line_top(int top, int width, int def_right) override;
    virtual void apply_vertical_align() override;
    virtual void draw_children(uintptr_t hdc,
        int x,
        int y,
        const Position* clip,
        DrawFlag flag,
        int zindex) override;
    virtual int get_zindex() const override;
    virtual void draw_stacking_context(uintptr_t hdc,
        int x,
        int y,
        const Position* clip,
        bool with_positioned) override;
    virtual void calc_document_size(litehtml::Size& sz, int x = 0, int y = 0) override;
    virtual void get_redraw_box(litehtml::Position& pos, int x = 0, int y = 0) override;
    virtual void add_style(const CSSStyle& style) override;
    virtual Element::ptr get_element_by_point(int x,
        int y,
        int client_x,
        int client_y) override;
    virtual Element::ptr get_child_by_point(int x,
        int y,
        int client_x,
        int client_y,
        DrawFlag flag,
        int zindex) override;

    virtual bool is_nth_child(const Element::ptr& el,
        int num,
        int off,
        bool of_type) const override;
    virtual bool is_nth_last_child(const Element::ptr& el,
        int num,
        int off,
        bool of_type) const override;
    virtual bool is_only_child(const Element::ptr& el, bool of_type) const override;
    virtual const Background* get_background(bool own_only = false) override;

protected:
    void draw_children_box(uintptr_t hdc,
        int x,
        int y,
        const Position* clip,
        DrawFlag flag,
        int zindex);
    void draw_children_table(uintptr_t hdc,
        int x,
        int y,
        const Position* clip,
        DrawFlag flag,
        int zindex);
    int render_box(int x, int y, int max_width, bool second_pass = false);
    int render_table(int x, int y, int max_width, bool second_pass = false);
    int fix_line_width(int max_width, ElementFloat flt);
    void parse_background();
    void init_BackgroundPaint(Position pos,
        BackgroundPaint& bg_paint,
        const Background* bg);
    void draw_list_marker(uintptr_t hdc, const Position& pos);
    tstring get_list_marker_text(int index);
    void parse_nth_child_params(tstring param, int& num, int& off);
    void remove_before_after();
    litehtml::Element::ptr get_element_before();
    litehtml::Element::ptr get_element_after();
};

/************************************************************************/
/*                        Inline Functions                              */
/************************************************************************/

} // namespace litehtml

#endif // LITEHTML_HTML_ELEMENT_H__
