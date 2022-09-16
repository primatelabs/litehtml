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

#include "litehtml/element/image_element.h"

#include "litehtml/document.h"
#include "litehtml/document_container.h"

namespace litehtml {

ImageElement::ImageElement(Document* doc)
: HTMLElement(doc)
{
    m_display = kDisplayInlineBlock;
}

ImageElement::~ImageElement(void)
{
}

Size ImageElement::get_content_size(int)
{
    return get_document()->container()->get_image_size(src_);
}

int ImageElement::calc_max_height(int image_height)
{
    Document* doc = get_document();
    int percentSize = 0;
    if (m_css_max_height.units() == kCSSUnitsPercent) {
        auto el_parent = parent();
        if (el_parent) {
            if (!el_parent->get_predefined_height(percentSize)) {
                return image_height;
            }
        }
    }
    return doc->cvt_units(m_css_max_height, font_size_, percentSize);
}

int ImageElement::line_height() const
{
    return height();
}

bool ImageElement::is_replaced() const
{
    return true;
}

int ImageElement::render(int x, int y, int parent_width, bool /* second_pass */)
{
    calc_outlines(parent_width);

    position_.move_to(x, y);

    Document* doc = get_document();
    DocumentContainer* ctr = doc->container();

    Size sz = ctr->get_image_size(src_);

    position_.width = sz.width;
    position_.height = sz.height;

    if (m_css_height.is_predefined() && m_css_width.is_predefined()) {
        position_.height = sz.height;
        position_.width = sz.width;

        // check for max-width
        if (!m_css_max_width.is_predefined()) {
            int max_width =
                doc->cvt_units(m_css_max_width, font_size_, parent_width);
            if (position_.width > max_width) {
                position_.width = max_width;
            }
            if (sz.width) {
                position_.height =
                    (int)((float)position_.width * (float)sz.height / (float)sz.width);
            } else {
                position_.height = sz.height;
            }
        }

        // check for max-height
        if (!m_css_max_height.is_predefined()) {
            int max_height = calc_max_height(sz.height);
            if (position_.height > max_height) {
                position_.height = max_height;
            }
            if (sz.height) {
                position_.width =
                    (int)(position_.height * (float)sz.width / (float)sz.height);
            } else {
                position_.width = sz.width;
            }
        }
    } else if (!m_css_height.is_predefined() && m_css_width.is_predefined()) {
        if (!get_predefined_height(position_.height)) {
            position_.height = (int)m_css_height.val();
        }

        // check for max-height
        if (!m_css_max_height.is_predefined()) {
            int max_height = calc_max_height(sz.height);
            if (position_.height > max_height) {
                position_.height = max_height;
            }
        }

        if (sz.height) {
            position_.width = (int)(position_.height * (float)sz.width / (float)sz.height);
        } else {
            position_.width = sz.width;
        }
    } else if (m_css_height.is_predefined() && !m_css_width.is_predefined()) {
        position_.width = (int)m_css_width.calc_percent(parent_width);

        // check for max-width
        if (!m_css_max_width.is_predefined()) {
            int max_width =
                doc->cvt_units(m_css_max_width, font_size_, parent_width);
            if (position_.width > max_width) {
                position_.width = max_width;
            }
        }

        if (sz.width) {
            position_.height =
                (int)((float)position_.width * (float)sz.height / (float)sz.width);
        } else {
            position_.height = sz.height;
        }
    } else {
        position_.width = (int)m_css_width.calc_percent(parent_width);
        position_.height = 0;
        if (!get_predefined_height(position_.height)) {
            position_.height = (int)m_css_height.val();
        }

        // check for max-height
        if (!m_css_max_height.is_predefined()) {
            int max_height = calc_max_height(sz.height);
            if (position_.height > max_height) {
                position_.height = max_height;
            }
        }

        // check for max-height
        if (!m_css_max_width.is_predefined()) {
            int max_width =
                doc->cvt_units(m_css_max_width, font_size_, parent_width);
            if (position_.width > max_width) {
                position_.width = max_width;
            }
        }
    }

    calc_auto_margins(parent_width);

    position_.x += content_margins_left();
    position_.y += content_margins_top();

    return position_.width + content_margins_left() + content_margins_right();
}

void ImageElement::parse_attributes()
{
    // Resolve the image URL using the document base URL.  Store the
    // absolute image URL rather than the (possibly) relative image URL so
    // we don't need to resolve the URL later.
    tstring src = get_attr(_t("src"), _t(""));
    if (!src.empty()) {
        src_ = resolve(get_document()->base_url(), URL(src));
    }

    const tchar_t* attr_height = get_attr(_t("height"));
    if (attr_height) {
        m_style.add_property(kCSSPropertyHeight, attr_height, URL(), false);
    }
    const tchar_t* attr_width = get_attr(_t("width"));
    if (attr_width) {
        m_style.add_property(kCSSPropertyWidth, attr_width, URL(), false);
    }
}

void ImageElement::draw(uintptr_t hdc, int x, int y, const Position* clip)
{
    Position pos = position_;
    pos.x += x;
    pos.y += y;

    Position el_pos = pos;
    el_pos += m_padding;
    el_pos += m_borders;

    // draw standard background here
    if (el_pos.does_intersect(clip)) {
        const Background* bg = get_background();
        if (bg) {
            BackgroundPaint bg_paint;
            init_BackgroundPaint(pos, bg_paint, bg);

            get_document()->container()->draw_background(hdc, bg_paint);
        }
    }

    // draw image as background
    if (pos.does_intersect(clip)) {
        if (pos.width > 0 && pos.height > 0) {
            BackgroundPaint bg;
            bg.image = src_;
            bg.clip_box = pos;
            bg.origin_box = pos;
            bg.border_box = pos;
            bg.border_box += m_padding;
            bg.border_box += m_borders;
            bg.repeat = kBackgroundRepeatNoRepeat;
            bg.image_size.width = pos.width;
            bg.image_size.height = pos.height;
            bg.border_radii = m_css_borders.radii.calculate_radii(bg.border_box.width, bg.border_box.height);
            bg.position_x = pos.x;
            bg.position_y = pos.y;
            get_document()->container()->draw_background(hdc, bg);
        }
    }

    // draw borders
    if (el_pos.does_intersect(clip)) {
        Position border_box = pos;
        border_box += m_padding;
        border_box += m_borders;

        Borders borders = m_css_borders.calculate_borders(border_box.width, border_box.height);

        get_document()->container()->draw_borders(hdc,
            borders,
            border_box,
            have_parent() ? false : true);
    }
}

void ImageElement::parse_styles(bool is_reparse /*= false*/)
{
    HTMLElement::parse_styles(is_reparse);

    if (!src_.empty()) {
        Document* document = get_document();
        DocumentContainer* container = document->container();

        if (!m_css_height.is_predefined() && !m_css_width.is_predefined()) {
            container->load_image(src_, true);
        } else {
            container->load_image(src_, false);
        }
    }
}

} // namespace litehtml
