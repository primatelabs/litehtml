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

#ifndef LITEHTML_BOX_H__
#define LITEHTML_BOX_H__

#include <memory>
#include <vector>

#include "litehtml/element/element.h"

namespace litehtml {

enum BoxType {
    kBoxBlock,
    kBoxLine
};

class Box {
public:
    typedef std::unique_ptr<Box> ptr;
    typedef std::vector<Box::ptr> vector;

protected:
    int m_box_top;
    int m_box_left;
    int m_box_right;

public:
    Box(int top, int left, int right)
    {
        m_box_top = top;
        m_box_left = left;
        m_box_right = right;
    }

    virtual ~Box();

    int bottom()
    {
        return m_box_top + height();
    }

    int top()
    {
        return m_box_top;
    }

    int right()
    {
        return m_box_left + width();
    }

    int left()
    {
        return m_box_left;
    }

    virtual litehtml::BoxType get_type() = 0;
    virtual int height() = 0;
    virtual int width() = 0;
    virtual void add_element(const Element::ptr& el) = 0;
    virtual bool can_hold(const Element::ptr& el, CSSKeyword ws) = 0;
    virtual void finish(bool last_box = false) = 0;
    virtual bool is_empty() = 0;
    virtual int baseline() = 0;
    virtual void get_elements(ElementsVector& els) = 0;
    virtual int top_margin() = 0;
    virtual int bottom_margin() = 0;
    virtual void y_shift(int shift) = 0;
    virtual void new_width(int left, int right, ElementsVector& els) = 0;
};

//////////////////////////////////////////////////////////////////////////

class BlockBox : public Box {
    Element::ptr m_element;

public:
    BlockBox(int top, int left, int right)
    : Box(top, left, right)
    , m_element(nullptr)
    {
    }

    virtual litehtml::BoxType get_type();
    virtual int height();
    virtual int width();
    virtual void add_element(const Element::ptr& el);
    virtual bool can_hold(const Element::ptr& el, CSSKeyword ws);
    virtual void finish(bool last_box = false);
    virtual bool is_empty();
    virtual int baseline();
    virtual void get_elements(ElementsVector& els);
    virtual int top_margin();
    virtual int bottom_margin();
    virtual void y_shift(int shift);
    virtual void new_width(int left, int right, ElementsVector& els);
};

//////////////////////////////////////////////////////////////////////////

class LineBox : public Box {
    ElementsVector m_items;
    int m_height;
    int m_width;
    int line_height_;
    FontMetrics font_metrics_;
    int m_baseline;
    TextAlign m_text_align;

public:
    LineBox(int top, int left, int right, int line_height, FontMetrics& fm, TextAlign align)
    : Box(top, left, right)
    {
        m_height = 0;
        m_width = 0;
        font_metrics_ = fm;
        line_height_ = line_height;
        m_baseline = 0;
        m_text_align = align;
    }

    virtual BoxType get_type();
    virtual int height();
    virtual int width();
    virtual void add_element(const Element::ptr& el);
    virtual bool can_hold(const Element::ptr& el, CSSKeyword ws);
    virtual void finish(bool last_box = false);
    virtual bool is_empty();
    virtual int baseline();
    virtual void get_elements(ElementsVector& els);
    virtual int top_margin();
    virtual int bottom_margin();
    virtual void y_shift(int shift);
    virtual void new_width(int left, int right, ElementsVector& els);

private:
    bool have_last_space();
    bool is_break_only();
};

} // namespace litehtml

#endif // LITEHTML_BOX_H__
