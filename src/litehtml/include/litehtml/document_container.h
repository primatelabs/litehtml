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

#ifndef LITEHTML_DOCUMENT_CONTAINER_H__
#define LITEHTML_DOCUMENT_CONTAINER_H__

#include <string>

#include "litehtml/background_paint.h"
#include "litehtml/color.h"
#include "litehtml/element/element.h"
#include "litehtml/list_marker.h"
#include "litehtml/url.h"

namespace litehtml {

// call back interface to draw text, images and other elements
class DocumentContainer {
public:
    virtual ~DocumentContainer() = default;

    virtual uintptr_t create_font(const litehtml::tchar_t* faceName,
        int size,
        int weight,
        litehtml::font_style italic,
        unsigned int decoration,
        litehtml::FontMetrics* fm) = 0;

    virtual void delete_font(uintptr_t hFont) = 0;

    virtual int text_width(const litehtml::tchar_t* text,
        uintptr_t hFont) = 0;

    virtual void draw_text(uintptr_t hdc,
        const litehtml::tchar_t* text,
        uintptr_t hFont,
        litehtml::Color color,
        const litehtml::Position& pos) = 0;

    virtual int pt_to_px(int pt) = 0;

    virtual int get_default_font_size() const = 0;

    virtual const litehtml::tchar_t* get_default_font_name() const = 0;

    virtual void draw_list_marker(uintptr_t hdc,
        const litehtml::list_marker& marker) = 0;

    virtual void load_image(const litehtml::URL& url,
        bool redraw_on_ready) = 0;

    virtual litehtml::Size get_image_size(const URL& url) = 0;

    virtual void draw_background(uintptr_t hdc,
        const litehtml::BackgroundPaint& bg) = 0;

    virtual void draw_borders(uintptr_t hdc,
        const litehtml::Borders& borders,
        const litehtml::Position& draw_pos,
        bool root) = 0;

    virtual void set_caption(const litehtml::tchar_t* caption) = 0;

    virtual void link(const Document* doc,
        const litehtml::Element::ptr& el) = 0;

    virtual void on_anchor_click(const litehtml::tchar_t* url,
        const litehtml::Element* el) = 0;

    virtual void set_cursor(const litehtml::tchar_t* cursor) = 0;

    virtual void transform_text(litehtml::tstring& text,
        litehtml::TextTransform tt) = 0;

    virtual tstring import_css(const URL& css_url) = 0;

    virtual void set_clip(const litehtml::Position& pos,
        const litehtml::BorderRadii& border_radii,
        bool valid_x,
        bool valid_y) = 0;

    virtual void del_clip() = 0;

    virtual litehtml::Position get_client_rect() const = 0;

    virtual void get_media_features(litehtml::MediaFeatures& media) const = 0;

    virtual void get_language(litehtml::tstring& language,
        litehtml::tstring& culture) const = 0;
};

} // namespace litehtml

#endif // LITEHTML_DOCUMENT_CONTAINER_H__
