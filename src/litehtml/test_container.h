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

#ifndef TEST_TEST_CONTAINER_H__
#define TEST_TEST_CONTAINER_H__

#include "litehtml/document_container.h"

class test_container : public litehtml::DocumentContainer {
public:
    test_container();

    virtual ~test_container();

    virtual uintptr_t create_font(const char* faceName,
        int size,
        int weight,
        litehtml::font_style italic,
        unsigned int decoration,
        litehtml::FontMetrics* fm) override;

    virtual void delete_font(uintptr_t hFont) override;

    virtual int text_width(const char* text,
        uintptr_t hFont) override;

    virtual void draw_text(uintptr_t hdc,
        const char* text,
        uintptr_t hFont,
        litehtml::Color color,
        const litehtml::Position& pos) override;

    virtual int pt_to_px(int pt) override;

    virtual int get_default_font_size() const override;

    virtual const char* get_default_font_name() const override;

    virtual void load_image(const litehtml::URL& src,
        bool redraw_on_ready) override;

    virtual litehtml::Size get_image_size(const litehtml::URL& src) override;

    virtual void draw_background(uintptr_t hdc,
        const litehtml::BackgroundPaint& bg) override;

    virtual void draw_borders(uintptr_t hdc,
        const litehtml::Borders& borders,
        const litehtml::Position& draw_pos,
        bool root) override;

    virtual void draw_list_marker(uintptr_t hdc,
        const litehtml::list_marker& marker) override;

    virtual void get_media_features(litehtml::MediaFeatures& media) const override;

    virtual void get_language(std::string& language,
        std::string& culture) const override;

    virtual void link(const litehtml::Document* ptr,
        const litehtml::Element::ptr& el) override;

    virtual void transform_text(std::string& text,
        litehtml::TextTransform tt) override;

    virtual void set_clip(const litehtml::Position& pos,
        const litehtml::BorderRadii& border_radii,
        bool valid_x,
        bool valid_y) override;

    virtual void del_clip() override;

    virtual void set_caption(const char* caption) override;

    virtual void on_anchor_click(const char* url,
        const litehtml::Element* el) override;

    virtual void set_cursor(const char* cursor) override;

    virtual std::string import_css(const litehtml::URL& url) override;

    virtual std::string import_js(const litehtml::URL& url) override;

    virtual litehtml::Position get_client_rect() const override;
};

#endif // TEST_TEST_CONTAINER_H__
