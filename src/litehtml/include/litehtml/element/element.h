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
#include "litehtml/debug/json.h"

namespace litehtml {

class Box;

enum ElementType {
    kElement,
    kElementAfter,
    kElementAnchor,
    kElementBase,
    kElementBefore,
    kElementBody,
    kElementBreak,
    kElementCDATA,
    kElementComment,
    kElementDiv,
    kElementFont,
    kElementHTML,
    kElementImage,
    kElementLI,
    kElementLink,
    kElementParagraph,
    kElementScript,
    kElementStyle,
    kElementTable,
    kElementTD,
    kElementText,
    kElementTitle,
    kElementTR,
    kElementWhitespace,
};

enum Direction {
    kDirectionLTR,
    kDirectionRTL,
    kDirectionAuto,
    kDirectionUndefined
};

enum Directionality {
    kDirectionalityLTR,
    kDirectionalityRTL,
};

String element_type_name(ElementType type);

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
    // Pointer to the document that contains the element.
    Document* m_doc;

    // Pointer to the parent element.
    Element* m_parent;

    // Pointer to the box that contains the element.
    Box* m_box;

    // Pointers to the children of the element.
    ElementsVector m_children;

    // Element position. The origin is unclear (it may be relative to the
    // parent element, it may be relative to the document, it may depend on
    // the CSS position attribute).
    Position position_;

    // CSS box model metrics (margin, border, and padding). Consider moving
    // from Element to HTMLElement since these only apply to visible elements.
    Margins margin_;

    Margins border_;

    Margins padding_;

    Direction direction_ = kDirectionUndefined;

    // Flag used by LineBox (and elsewhere). May control visibility since
    // skip is always true for certain elements (e.g., comments).
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
        return position_.left() - margin_.left - padding_.left - border_.left;
    }

    int right() const
    {
        return left() + width();
    }

    int top() const
    {
        return position_.top() - margin_.top - padding_.top - border_.top;
    }

    int bottom() const
    {
        return top() + height();
    }

    int height() const
    {
        return position_.height + margin_.top + margin_.bottom + padding_.height() +
               border_.height();
    }

    int width() const
    {
        return position_.width + margin_.left + margin_.right + padding_.width() +
               border_.width();
    }

    int content_margin_top() const
    {
        return margin_.top + padding_.top + border_.top;
    }

     int content_margin_bottom() const
    {
        return margin_.bottom + padding_.bottom + border_.bottom;
    }

     int content_margin_left() const
    {
        return margin_.left + padding_.left + border_.left;
    }

     int content_margin_right() const
    {
        return margin_.right + padding_.right + border_.right;
    }

     int content_margin_width() const
    {
        return content_margin_left() + content_margin_right();
    }

     int content_margin_height() const
    {
        return content_margin_top() + content_margin_bottom();
    }

    litehtml::Margins margin() const
    {
        return margin_;
    }

    litehtml::Margins padding() const
    {
        return padding_;
    }

    litehtml::Margins border() const
    {
        return border_;
    }

    Direction get_direction() const
    {
        return direction_;
    }

    Directionality get_directionality() const;

    Directionality get_parent_directionality() const;

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

    virtual ElementsVector select_all(const std::string& selector);
    virtual ElementsVector select_all(const CSSSelector& selector);

    virtual Element::ptr select_one(const std::string& selector);
    virtual Element::ptr select_one(const CSSSelector& selector);

    virtual int render(int x, int y, int max_width, bool second_pass = false);
    virtual int render_inline(const ptr& container, int max_width);
    virtual int place_element(const ptr& el, int max_width);
    virtual void calc_outlines(int parent_width);
    virtual void calc_auto_margins(int parent_width);
    virtual void apply_vertical_align();
    virtual bool fetch_positioned();
    virtual void render_positioned();

    // Returns true if the parent elemenet appends the child element and
    // adopts ownership of it, false otherwise.
    virtual bool append_child(Element* element);

    // For each of the child elements either append and adopt the child
    // element or delete the element.
    virtual void append_children(ElementsVector& children);

    virtual ElementType type() const;

    String type_name() const;

    virtual const char* get_tagName() const;
    virtual void set_tagName(const char* tag);
    virtual void set_data(const char* data);
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

    virtual void set_attr(const char* name, const char* val);
    virtual const char* get_attr(const char* name,
        const char* def = nullptr) const;
    virtual void apply_stylesheet(const CSSStylesheet& stylesheet);
    virtual void refresh_styles();
    virtual bool is_whitespace() const;
    virtual bool is_body() const;
    virtual bool is_break() const;
    virtual int get_baseline();
    virtual bool on_mouse_over();
    virtual bool on_mouse_leave();
    virtual bool on_lbutton_down();
    virtual bool on_lbutton_up();
    virtual void on_click();
    virtual bool find_styles_changes(std::vector<Position>& redraw_boxes, int x, int y);
    virtual const char* get_cursor();
    virtual void init_font();
    virtual bool is_point_inside(int x, int y);
    virtual bool set_pseudo_class(const char* pclass, bool add);
    virtual bool set_class(const char* pclass, bool add);
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

    virtual const char* get_style_property(CSSProperty name);

    virtual const CSSValue* get_style_property_value(CSSProperty property) const;

    virtual uintptr_t get_font(FontMetrics* fm = nullptr);
    virtual int get_font_size() const;
    virtual void get_text(std::string& text) const;
    virtual void parse_attributes();
    virtual int select(const CSSSelector& selector, bool apply_pseudo = true);
    virtual int select(const CSSElementSelector& selector,
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

#if defined(ENABLE_JSON)
    virtual nlohmann::json json() const;
#endif // ENABLE_JSON
};

} // namespace litehtml

#endif // LITEHTML_ELEMENT_H__
