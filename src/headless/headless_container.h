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

#ifndef HEADLESS_HEADLESS_CONTAINER_H__
#define HEADLESS_HEADLESS_CONTAINER_H__

#include "freetype.h"
#include "image/image.h"
#include "litehtml/litehtml.h"
#include "litehtml/url.h"

namespace headless {

class HeadlessContainer : public litehtml::DocumentContainer {
public:
    FT_Library library_;

    int dpi_;

    // litehtml does not maintain its own image cache.  Instead, it notifies the
    // container when it encounters an image during parsing by calling
    // load_image().  Then it notifies the container to draw the image during
    // rendering by calling draw_background().  It's not clear if litehtml will
    // ever attempt to render an image without loading it first (i.e., will it
    // call draw_background() without a matching load_image() call).  For now
    // we assume it will not.
    //
    // HeadlessContainer loads the images during load_image() and caches them in
    // the image cache.  HeadlessContainer loads images from the image cache for
    // get_image_size() and draw_background() calls.  HeadlessContainer will not
    // attempt to load images in either get_image_size() or draw_background().

    std::unordered_map<std::string, Image<uint8_t>> image_cache_;

    HeadlessContainer();

    virtual ~HeadlessContainer();

    virtual uintptr_t create_font(const litehtml::tchar_t* faceName,
        int size,
        int weight,
        litehtml::font_style italic,
        unsigned int decoration,
        litehtml::FontMetrics* fm) override;

    virtual void delete_font(uintptr_t hFont) override;

    virtual int text_width(const litehtml::tchar_t* text,
        uintptr_t hFont) override;

    virtual void draw_text(uintptr_t hdc,
        const litehtml::tchar_t* text,
        uintptr_t hFont,
        litehtml::Color color,
        const litehtml::Position& pos) override;

    virtual int pt_to_px(int pt) override;

    virtual int get_default_font_size() const override;

    virtual const litehtml::tchar_t* get_default_font_name() const override;

    virtual void load_image(const litehtml::URL& src, bool redraw_on_ready) override;

    virtual litehtml::Size get_image_size(const litehtml::URL& url) override;

    virtual void draw_background(uintptr_t hdc,
        const litehtml::BackgroundPaint& bg) override;

    virtual void draw_borders(uintptr_t hdc,
        const litehtml::Borders& borders,
        const litehtml::Position& draw_pos,
        bool root) override;

    virtual void draw_list_marker(uintptr_t hdc,
        const litehtml::list_marker& marker) override;

    virtual void get_media_features(litehtml::MediaFeatures& media) const override;

    virtual void get_language(litehtml::tstring& language,
        litehtml::tstring& culture) const override;

    virtual void link(const litehtml::Document* ptr,
        const litehtml::Element::ptr& el) override;

    virtual void transform_text(litehtml::tstring& text,
        litehtml::TextTransform tt) override;

    virtual void set_clip(const litehtml::Position& pos,
        const litehtml::BorderRadii& border_radii,
        bool valid_x,
        bool valid_y) override;

    virtual void del_clip() override;

    virtual void set_caption(const litehtml::tchar_t* caption) override;

    virtual void on_anchor_click(const litehtml::tchar_t* url,
        const litehtml::Element* el) override;

    virtual void set_cursor(const litehtml::tchar_t* cursor) override;

    virtual litehtml::tstring import_css(const litehtml::URL& url) override;

    virtual void get_client_rect(litehtml::Position& client) const override;
};

} // namespace headless

#endif // HEADLESS_HEADLESS_CONTAINER_H__
