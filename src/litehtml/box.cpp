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

#include "litehtml/box.h"

#include "litehtml/html.h"
#include "litehtml/element/html_element.h"

namespace litehtml {

Box::~Box()
{
}

BoxType BlockBox::get_type()
{
    return kBoxBlock;
}

int BlockBox::height()
{
    return m_element->height();
}

int BlockBox::width()
{
    return m_element->width();
}

void BlockBox::add_element(const Element::ptr& el)
{
    m_element = el;
    el->m_box = this;
}

void BlockBox::finish(bool)
{
    if (!m_element) {
        return;
    }
    m_element->apply_relative_shift(m_box_right - m_box_left);
}

bool BlockBox::can_hold(const Element::ptr& el, CSSKeyword)
{
    if (m_element || el->is_inline_box()) {
        return false;
    }
    return true;
}

bool BlockBox::is_empty()
{
    if (m_element) {
        return false;
    }
    return true;
}

int BlockBox::baseline()
{
    if (m_element) {
        return m_element->get_baseline();
    }
    return 0;
}

void BlockBox::get_elements(ElementsVector& els)
{
    els.push_back(m_element);
}

int BlockBox::top_margin()
{
    if (m_element && m_element->collapse_top_margin()) {
        return m_element->m_margins.top;
    }
    return 0;
}

int BlockBox::bottom_margin()
{
    if (m_element && m_element->collapse_bottom_margin()) {
        return m_element->m_margins.bottom;
    }
    return 0;
}

void BlockBox::y_shift(int shift)
{
    m_box_top += shift;
    if (m_element) {
        m_element->position_.y += shift;
    }
}

void BlockBox::new_width(int, int, ElementsVector&)
{
}

//////////////////////////////////////////////////////////////////////////

BoxType LineBox::get_type()
{
    return kBoxLine;
}

int LineBox::height()
{
    return m_height;
}

int LineBox::width()
{
    return m_width;
}

void LineBox::add_element(const Element::ptr& el)
{
    el->m_skip = false;
    el->m_box = nullptr;
    bool add = true;
    if ((m_items.empty() && el->is_white_space()) || el->is_break()) {
        el->m_skip = true;
    } else if (el->is_white_space()) {
        if (have_last_space()) {
            add = false;
            el->m_skip = true;
        }
    }

    if (add) {
        el->m_box = this;
        m_items.push_back(el);

        if (!el->m_skip) {
            int el_shift_left = el->get_inline_shift_left();
            int el_shift_right = el->get_inline_shift_right();

            el->position_.x = m_box_left + m_width + el_shift_left +
                          el->content_margins_left();
            el->position_.y = m_box_top + el->content_margins_top();
            m_width += el->width() + el_shift_left + el_shift_right;
        }
    }
}

void LineBox::finish(bool last_box)
{
    if (is_empty() || (!is_empty() && last_box && is_break_only())) {
        m_height = 0;
        return;
    }

    for (auto i = m_items.rbegin(); i != m_items.rend(); i++) {
        if ((*i)->is_white_space() || (*i)->is_break()) {
            if (!(*i)->m_skip) {
                (*i)->m_skip = true;
                m_width -= (*i)->width();
            }
        } else {
            break;
        }
    }

    int baseline = font_metrics_.baseline();
    int line_height = line_height_;

    int add_x = 0;
    switch (m_text_align) {
        case kTextAlignRight:
            if (m_width < (m_box_right - m_box_left)) {
                add_x = (m_box_right - m_box_left) - m_width;
            }
            break;
        case kTextAlignCenter:
            if (m_width < (m_box_right - m_box_left)) {
                add_x = ((m_box_right - m_box_left) - m_width) / 2;
            }
            break;
        default:
            add_x = 0;
    }

    m_height = 0;
    // find line box baseline and line-height
    for (const auto& el : m_items) {
        if (el->get_display() == kDisplayInlineText) {
            FontMetrics fm;
            el->get_font(&fm);
            baseline = std::max(baseline, fm.baseline());
            line_height = std::max(line_height, el->line_height());
            m_height = std::max(m_height, fm.height);
        }
        el->position_.x += add_x;
    }

    if (m_height) {
        baseline += (line_height - m_height) / 2;
    }

    m_height = line_height;

    int y1 = 0;
    int y2 = m_height;

    for (const auto& el : m_items) {
        if (el->get_display() == kDisplayInlineText) {
            FontMetrics fm;
            el->get_font(&fm);
            el->position_.y = m_height - baseline - fm.ascent;
        } else {
            switch (el->get_vertical_align()) {
                case kVerticalAlignSuper:
                case kVerticalAlignSub:
                case kVerticalAlignBaseline:
                    el->position_.y = m_height - baseline - el->height() +
                                  el->get_baseline() + el->content_margins_top();
                    break;
                case kVerticalAlignTop:
                    el->position_.y = y1 + el->content_margins_top();
                    break;
                case kVerticalAlignTextTop:
                    el->position_.y = m_height - baseline - font_metrics_.ascent +
                                  el->content_margins_top();
                    break;
                case kVerticalAlignMiddle:
                    el->position_.y = m_height - baseline -
                                  font_metrics_.x_height / 2 -
                                  el->height() / 2 + el->content_margins_top();
                    break;
                case kVerticalAlignBottom:
                    el->position_.y = y2 - el->height() + el->content_margins_top();
                    break;
                case kVerticalAlignTextBottom:
                    el->position_.y = m_height - baseline + font_metrics_.descent -
                                  el->height() + el->content_margins_top();
                    break;
            }
            y1 = std::min(y1, el->top());
            y2 = std::max(y2, el->bottom());
        }
    }

    CSSOffsets offsets;

    for (const auto& el : m_items) {
        el->position_.y -= y1;
        el->position_.y += m_box_top;
        if (el->get_display() != kDisplayInlineText) {
            switch (el->get_vertical_align()) {
                case kVerticalAlignTop:
                    el->position_.y = m_box_top + el->content_margins_top();
                    break;
                case kVerticalAlignBottom:
                    el->position_.y = m_box_top + (y2 - y1) - el->height() +
                                  el->content_margins_top();
                    break;
                case kVerticalAlignBaseline:
                    // TODO: process vertical align "baseline"
                    break;
                case kVerticalAlignMiddle:
                    // TODO: process vertical align "middle"
                    break;
                case kVerticalAlignSub:
                    // TODO: process vertical align "sub"
                    break;
                case kVerticalAlignSuper:
                    // TODO: process vertical align "super"
                    break;
                case kVerticalAlignTextBottom:
                    // TODO: process vertical align "text-bottom"
                    break;
                case kVerticalAlignTextTop:
                    // TODO: process vertical align "text-top"
                    break;
            }
        }

        el->apply_relative_shift(m_box_right - m_box_left);
    }
    m_height = y2 - y1;
    m_baseline = (baseline - y1) - (m_height - line_height);
}

bool LineBox::can_hold(const Element::ptr& el, CSSKeyword ws)
{
    if (!el->is_inline_box())
        return false;

    if (el->is_break()) {
        return false;
    }

    if (ws == kWhiteSpaceNowrap || ws == kWhiteSpacePre) {
        return true;
    }

    if (m_box_left + m_width + el->width() + el->get_inline_shift_left() +
            el->get_inline_shift_right() >
        m_box_right) {
        return false;
    }

    return true;
}

bool LineBox::have_last_space()
{
    bool ret = false;
    for (auto i = m_items.rbegin(); i != m_items.rend() && !ret; i++) {
        if ((*i)->is_white_space() || (*i)->is_break()) {
            ret = true;
        } else {
            break;
        }
    }
    return ret;
}

bool LineBox::is_empty()
{
    if (m_items.empty())
        return true;
    for (auto i = m_items.rbegin(); i != m_items.rend(); i++) {
        if (!(*i)->m_skip || (*i)->is_break()) {
            return false;
        }
    }
    return true;
}

int LineBox::baseline()
{
    return m_baseline;
}

void LineBox::get_elements(ElementsVector& els)
{
    els.insert(els.begin(), m_items.begin(), m_items.end());
}

int LineBox::top_margin()
{
    return 0;
}

int LineBox::bottom_margin()
{
    return 0;
}

void LineBox::y_shift(int shift)
{
    m_box_top += shift;
    for (auto& el : m_items) {
        el->position_.y += shift;
    }
}

bool LineBox::is_break_only()
{
    if (m_items.empty())
        return true;

    if (m_items.front()->is_break()) {
        for (auto& el : m_items) {
            if (!el->m_skip) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void LineBox::new_width(int left, int right, ElementsVector& els)
{
    int add = left - m_box_left;
    if (add) {
        m_box_left = left;
        m_box_right = right;
        m_width = 0;
        auto remove_begin = m_items.end();
        for (auto i = m_items.begin() + 1; i != m_items.end(); i++) {
            Element::ptr el = (*i);

            if (!el->m_skip) {
                if (m_box_left + m_width + el->width() +
                        el->get_inline_shift_right() + el->get_inline_shift_left() >
                    m_box_right) {
                    remove_begin = i;
                    break;
                } else {
                    el->position_.x += add;
                    m_width += el->width() + el->get_inline_shift_right() +
                               el->get_inline_shift_left();
                }
            }
        }
        if (remove_begin != m_items.end()) {
            els.insert(els.begin(), remove_begin, m_items.end());
            m_items.erase(remove_begin, m_items.end());

            for (const auto& el : els) {
                el->m_box = nullptr;
            }
        }
    }
}

} // namespace litehtml
