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

#include "litehtml/element/el_image.h"

#include "litehtml/document.h"
#include "litehtml/document_container.h"

namespace litehtml {

el_image::el_image(const std::shared_ptr<document>& doc)
: html_tag(doc)
{
    m_display = display_inline_block;
}

el_image::~el_image(void)
{
}

void el_image::get_content_size(size& sz, int)
{
    get_document()->container()->get_image_size(m_src.c_str(), nullptr, sz);
}

int el_image::calc_max_height(int image_height)
{
    document::ptr doc = get_document();
    int percentSize = 0;
    if (m_css_max_height.units() == css_units_percentage) {
        auto el_parent = parent();
        if (el_parent) {
            if (!el_parent->get_predefined_height(percentSize)) {
                return image_height;
            }
        }
    }
    return doc->cvt_units(m_css_max_height, m_font_size, percentSize);
}

int el_image::line_height() const
{
    return height();
}

bool el_image::is_replaced() const
{
    return true;
}

int el_image::render(int x, int y, int parent_width, bool /* second_pass */)
{
    calc_outlines(parent_width);

    m_pos.move_to(x, y);

    document::ptr doc = get_document();
    document_container* ctr = doc->container();

    size sz = ctr->get_image_size(URL(m_src));

    m_pos.width = sz.width;
    m_pos.height = sz.height;

    if (m_css_height.is_predefined() && m_css_width.is_predefined()) {
        m_pos.height = sz.height;
        m_pos.width = sz.width;

        // check for max-width
        if (!m_css_max_width.is_predefined()) {
            int max_width =
                doc->cvt_units(m_css_max_width, m_font_size, parent_width);
            if (m_pos.width > max_width) {
                m_pos.width = max_width;
            }
            if (sz.width) {
                m_pos.height =
                    (int)((float)m_pos.width * (float)sz.height / (float)sz.width);
            } else {
                m_pos.height = sz.height;
            }
        }

        // check for max-height
        if (!m_css_max_height.is_predefined()) {
            int max_height = calc_max_height(sz.height);
            if (m_pos.height > max_height) {
                m_pos.height = max_height;
            }
            if (sz.height) {
                m_pos.width =
                    (int)(m_pos.height * (float)sz.width / (float)sz.height);
            } else {
                m_pos.width = sz.width;
            }
        }
    } else if (!m_css_height.is_predefined() && m_css_width.is_predefined()) {
        if (!get_predefined_height(m_pos.height)) {
            m_pos.height = (int)m_css_height.val();
        }

        // check for max-height
        if (!m_css_max_height.is_predefined()) {
            int max_height = calc_max_height(sz.height);
            if (m_pos.height > max_height) {
                m_pos.height = max_height;
            }
        }

        if (sz.height) {
            m_pos.width = (int)(m_pos.height * (float)sz.width / (float)sz.height);
        } else {
            m_pos.width = sz.width;
        }
    } else if (m_css_height.is_predefined() && !m_css_width.is_predefined()) {
        m_pos.width = (int)m_css_width.calc_percent(parent_width);

        // check for max-width
        if (!m_css_max_width.is_predefined()) {
            int max_width =
                doc->cvt_units(m_css_max_width, m_font_size, parent_width);
            if (m_pos.width > max_width) {
                m_pos.width = max_width;
            }
        }

        if (sz.width) {
            m_pos.height =
                (int)((float)m_pos.width * (float)sz.height / (float)sz.width);
        } else {
            m_pos.height = sz.height;
        }
    } else {
        m_pos.width = (int)m_css_width.calc_percent(parent_width);
        m_pos.height = 0;
        if (!get_predefined_height(m_pos.height)) {
            m_pos.height = (int)m_css_height.val();
        }

        // check for max-height
        if (!m_css_max_height.is_predefined()) {
            int max_height = calc_max_height(sz.height);
            if (m_pos.height > max_height) {
                m_pos.height = max_height;
            }
        }

        // check for max-height
        if (!m_css_max_width.is_predefined()) {
            int max_width =
                doc->cvt_units(m_css_max_width, m_font_size, parent_width);
            if (m_pos.width > max_width) {
                m_pos.width = max_width;
            }
        }
    }

    calc_auto_margins(parent_width);

    m_pos.x += content_margins_left();
    m_pos.y += content_margins_top();

    return m_pos.width + content_margins_left() + content_margins_right();
}

void el_image::parse_attributes()
{
    m_src = get_attr(_t("src"), _t(""));

    const tchar_t* attr_height = get_attr(_t("height"));
    if (attr_height) {
        m_style.add_property(_t("height"), attr_height, URL(), false);
    }
    const tchar_t* attr_width = get_attr(_t("width"));
    if (attr_width) {
        m_style.add_property(_t("width"), attr_width, URL(), false);
    }
}

void el_image::draw(uint_ptr hdc, int x, int y, const position* clip)
{
    position pos = m_pos;
    pos.x += x;
    pos.y += y;

    position el_pos = pos;
    el_pos += m_padding;
    el_pos += m_borders;

    // draw standard background here
    if (el_pos.does_intersect(clip)) {
        const background* bg = get_background();
        if (bg) {
            background_paint bg_paint;
            init_background_paint(pos, bg_paint, bg);

            get_document()->container()->draw_background(hdc, bg_paint);
        }
    }

    // draw image as background
    if (pos.does_intersect(clip)) {
        if (pos.width > 0 && pos.height > 0) {
            background_paint bg;
            bg.image = m_src;
            bg.clip_box = pos;
            bg.origin_box = pos;
            bg.border_box = pos;
            bg.border_box += m_padding;
            bg.border_box += m_borders;
            bg.repeat = background_repeat_no_repeat;
            bg.image_size.width = pos.width;
            bg.image_size.height = pos.height;
            bg.border_radius =
                m_css_borders.radius.calc_percents(bg.border_box.width,
                    bg.border_box.height);
            bg.position_x = pos.x;
            bg.position_y = pos.y;
            get_document()->container()->draw_background(hdc, bg);
        }
    }

    // draw borders
    if (el_pos.does_intersect(clip)) {
        position border_box = pos;
        border_box += m_padding;
        border_box += m_borders;

        borders bdr = m_css_borders;
        bdr.radius = m_css_borders.radius.calc_percents(border_box.width,
            border_box.height);

        get_document()->container()->draw_borders(hdc,
            bdr,
            border_box,
            have_parent() ? false : true);
    }
}

void el_image::parse_styles(bool is_reparse /*= false*/)
{
    html_tag::parse_styles(is_reparse);

    if (!m_src.empty()) {
        if (!m_css_height.is_predefined() && !m_css_width.is_predefined()) {
            get_document()->container()->load_image(URL(m_src), true);
        } else {
            get_document()->container()->load_image(URL(m_src), false);
        }
    }
}

} // namespace litehtml
