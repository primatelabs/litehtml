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

#ifndef LITEHTML_ELEMENT_H__
#define LITEHTML_ELEMENT_H__

#include <memory>

#include "litehtml/background.h"
#include "litehtml/color.h"
#include "litehtml/css/css_offsets.h"
#include "litehtml/css/css_stylesheet.h"

namespace litehtml {

class Box;

class Element : public std::enable_shared_from_this<Element> {
    friend class BlockBox;
    friend class LineBox;
    friend class HTMLElement;
    friend class TableElement;
    friend class Document;

public:
    typedef Element* ptr;
    typedef const Element* const_ptr;
    typedef std::weak_ptr<litehtml::Element> weak_ptr;

protected:
    Document* m_doc;

    Element* m_parent;

    Box* m_box;
    ElementsVector m_children;
    Position position_;
    margins m_margins;
    margins m_padding;
    margins m_borders;
    bool m_skip;

    virtual void select_all(const CSSSelector& selector, ElementsVector& res);

public:
    Element() = delete;

    explicit Element(Document* document);

    virtual ~Element();

    Position& get_position()
    {
      return position_;
    }

    int left() const
    {
        return position_.left() - margin_left() - m_padding.left - m_borders.left;
    }

    int right() const
    {
        return left() + width();
    }

    int top() const
    {
        return position_.top() - margin_top() - m_padding.top - m_borders.top;
    }

    int bottom() const
    {
        return top() + height();
    }

    int height() const
    {
        return position_.height + margin_top() + margin_bottom() + m_padding.height() +
               m_borders.height();
    }

    int width() const
    {
        return position_.width + margin_left() + margin_right() + m_padding.width() +
               m_borders.width();
    }

    int content_margins_top() const
    {
        return margin_top() + m_padding.top + m_borders.top;
    }

     int content_margins_bottom() const
    {
        return margin_bottom() + m_padding.bottom + m_borders.bottom;
    }

     int content_margins_left() const
    {
        return margin_left() + m_padding.left + m_borders.left;
    }

     int content_margins_right() const
    {
        return margin_right() + m_padding.right + m_borders.right;
    }

     int content_margins_width() const
    {
        return content_margins_left() + content_margins_right();
    }

     int content_margins_height() const
    {
        return content_margins_top() + content_margins_bottom();
    }

    litehtml::margins get_margins() const
    {
        margins ret;
        ret.left = margin_left();
        ret.right = margin_right();
        ret.top = margin_top();
        ret.bottom = margin_bottom();

        return ret;
    }

    int margin_top() const
    {
        return m_margins.top;
    }

    int margin_bottom() const
    {
        return m_margins.bottom;
    }

    int margin_left() const
    {
        return m_margins.left;
    }

    int margin_right() const
    {
        return m_margins.right;
    }

    litehtml::margins get_paddings() const
    {
        return m_padding;
    }

    int padding_top() const
    {
        return m_padding.top;
    }

    int padding_bottom() const
    {
        return m_padding.bottom;
    }

    int padding_left() const
    {
        return m_padding.left;
    }

    int padding_right() const
    {
        return m_padding.right;
    }


    litehtml::margins get_borders() const
    {
        return m_borders;
    }

    int border_top() const
    {
        return m_borders.top;
    }

    int border_bottom() const
    {
        return m_borders.bottom;
    }

    int border_left() const
    {
        return m_borders.left;
    }

    int border_right() const
    {
        return m_borders.right;
    }

    bool in_normal_flow() const
    {
        if (get_element_position() != kPositionAbsolute &&
            get_display() != kDisplayNone) {
            return true;
        }
        return false;
    }

    Color get_color(CSSProperty name, const Color& default_color = litehtml::Color());

    template <typename T>
    T get_keyword(CSSProperty name)
    {
        const CSSValue* value = get_style_property_value(name);
        const CSSKeywordValue* keyword_value = static_cast<const CSSKeywordValue*>(value);
        return static_cast<T>(keyword_value->keyword());
    }

    CSSLength get_length(CSSProperty name) const;

    bool is_inline_box() const;

    Position get_placement() const;

    bool collapse_top_margin() const;

    bool collapse_bottom_margin() const;

    bool is_positioned() const
    {
        return (get_element_position() > kPositionStatic);
    }

    bool skip()
    {
        return m_skip;
    }

    void skip(bool val)
    {
        m_skip = val;
    }

    bool have_parent() const
    {
        return m_parent != nullptr;
    }

    Element* parent() const
    {
        return m_parent;
    }

    void parent(Element* parent)
    {
        m_parent = parent;
    }

    bool is_visible() const
    {
        return !(m_skip || get_display() == kDisplayNone ||
                 get_visibility() != kVisibilityVisible);
    }

    int calc_width(int defVal) const;
    int get_inline_shift_left();
    int get_inline_shift_right();
    void apply_relative_shift(int parent_width);

    Document* get_document()
    {
        return m_doc;
    }

    const Document* get_document() const
    {
        return m_doc;
    }

    virtual ElementsVector select_all(const tstring& selector);
    virtual ElementsVector select_all(const CSSSelector& selector);

    virtual Element::ptr select_one(const tstring& selector);
    virtual Element::ptr select_one(const CSSSelector& selector);

    virtual int render(int x, int y, int max_width, bool second_pass = false);
    virtual int render_inline(const ptr& container, int max_width);
    virtual int place_element(const ptr& el, int max_width);
    virtual void calc_outlines(int parent_width);
    virtual void calc_auto_margins(int parent_width);
    virtual void apply_vertical_align();
    virtual bool fetch_positioned();
    virtual void render_positioned(RenderType rt = kRenderAll);

    // Returns true if the parent elemenet appends the child element and
    // adopts ownership of it, false otherwise.
    virtual bool append_child(Element* element);

    // For each of the child elements either append and adopt the child
    // element or delete the element.
    virtual void append_children(ElementsVector& children);

    virtual const tchar_t* get_tagName() const;
    virtual void set_tagName(const tchar_t* tag);
    virtual void set_data(const tchar_t* data);
    virtual ElementFloat get_float() const;
    virtual VerticalAlign get_vertical_align() const;
    virtual ElementClear get_clear() const;
    virtual size_t get_children_count() const;
    virtual Element::ptr get_child(int idx) const;
    virtual Overflow get_overflow() const;

    virtual CSSLength get_css_left() const;
    virtual CSSLength get_css_right() const;
    virtual CSSLength get_css_top() const;
    virtual CSSLength get_css_bottom() const;
    virtual CSSOffsets get_css_offsets() const;
    virtual CSSLength get_css_width() const;
    virtual void set_css_width(CSSLength& w);
    virtual CSSLength get_css_height() const;

    virtual void set_attr(const tchar_t* name, const tchar_t* val);
    virtual const tchar_t* get_attr(const tchar_t* name,
        const tchar_t* def = nullptr) const;
    virtual void apply_stylesheet(const CSSStylesheet& stylesheet);
    virtual void refresh_styles();
    virtual bool is_white_space() const;
    virtual bool is_body() const;
    virtual bool is_break() const;
    virtual int get_baseline();
    virtual bool on_mouse_over();
    virtual bool on_mouse_leave();
    virtual bool on_lbutton_down();
    virtual bool on_lbutton_up();
    virtual void on_click();
    virtual bool find_styles_changes(std::vector<Position>& redraw_boxes, int x, int y);
    virtual const tchar_t* get_cursor();
    virtual void init_font();
    virtual bool is_point_inside(int x, int y);
    virtual bool set_pseudo_class(const tchar_t* pclass, bool add);
    virtual bool set_class(const tchar_t* pclass, bool add);
    virtual bool is_replaced() const;
    virtual int line_height() const;
    virtual WhiteSpace get_white_space() const;
    virtual Display get_display() const;
    virtual Visibility get_visibility() const;
    virtual ElementPosition get_element_position(CSSOffsets* offsets = nullptr) const;
    virtual void get_inline_boxes(std::vector<Position>& boxes);
    virtual void parse_styles(bool is_reparse = false);
    virtual void draw(uintptr_t hdc, int x, int y, const Position* clip);
    virtual void draw_background(uintptr_t hdc, int x, int y, const Position* clip);

    virtual const tchar_t* get_style_property(CSSProperty name);

    virtual const CSSValue* get_style_property_value(CSSProperty property) const;

    virtual uintptr_t get_font(FontMetrics* fm = nullptr);
    virtual int get_font_size() const;
    virtual void get_text(tstring& text) const;
    virtual void parse_attributes();
    virtual int select(const CSSSelector& selector, bool apply_pseudo = true);
    virtual int select(const css_element_selector& selector,
        bool apply_pseudo = true);
    virtual Element::ptr find_ancestor(const CSSSelector& selector,
        bool apply_pseudo = true,
        bool* is_pseudo = nullptr);
    virtual bool is_ancestor(const ptr& el) const;
    virtual Element::ptr find_adjacent_sibling(const Element::ptr& el,
        const CSSSelector& selector,
        bool apply_pseudo = true,
        bool* is_pseudo = nullptr);
    virtual Element::ptr find_sibling(const Element::ptr& el,
        const CSSSelector& selector,
        bool apply_pseudo = true,
        bool* is_pseudo = nullptr);
    virtual bool is_first_child_inline(const Element::ptr& el) const;
    virtual bool is_last_child_inline(const Element::ptr& el);
    virtual bool have_inline_child() const;
    virtual Size get_content_size(int max_width);
    virtual void init();
    virtual bool is_floats_holder() const;
    virtual int get_floats_height(ElementFloat el_float = kFloatNone) const;
    virtual int get_left_floats_height() const;
    virtual int get_right_floats_height() const;
    virtual int get_line_left(int y);
    virtual int get_line_right(int y, int def_right);
    virtual void get_line_left_right(int y, int def_right, int& ln_left, int& ln_right);
    virtual void add_float(const ptr& el, int x, int y);
    virtual void update_floats(int dy, const ptr& parent);
    virtual void add_positioned(const ptr& el);
    virtual int find_next_line_top(int top, int width, int def_right);
    virtual int get_zindex() const;
    virtual void draw_stacking_context(uintptr_t hdc,
        int x,
        int y,
        const Position* clip,
        bool with_positioned);
    virtual void draw_children(uintptr_t hdc,
        int x,
        int y,
        const Position* clip,
        DrawFlag flag,
        int zindex);
    virtual bool is_nth_child(const Element::ptr& el,
        int num,
        int off,
        bool of_type) const;
    virtual bool is_nth_last_child(const Element::ptr& el,
        int num,
        int off,
        bool of_type) const;
    virtual bool is_only_child(const Element::ptr& el, bool of_type) const;
    virtual bool get_predefined_height(int& p_height) const;
    virtual void calc_document_size(litehtml::Size& sz, int x = 0, int y = 0);
    virtual void get_redraw_box(litehtml::Position& pos, int x = 0, int y = 0);
    virtual void add_style(const CSSStyle& style);
    virtual Element::ptr get_element_by_point(int x, int y, int client_x, int client_y);
    virtual Element::ptr get_child_by_point(int x,
        int y,
        int client_x,
        int client_y,
        DrawFlag flag,
        int zindex);
    virtual const Background* get_background(bool own_only = false);
};


} // namespace litehtml

#endif // LITEHTML_ELEMENT_H__
