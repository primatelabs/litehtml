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

class test_container : public litehtml::document_container {
public:
    test_container();

    virtual ~test_container();

    virtual litehtml::uint_ptr create_font(const litehtml::tchar_t* faceName,
        int size,
        int weight,
        litehtml::font_style italic,
        unsigned int decoration,
        litehtml::font_metrics* fm) override;

    virtual void delete_font(litehtml::uint_ptr hFont) override;

    virtual int text_width(const litehtml::tchar_t* text,
        litehtml::uint_ptr hFont) override;

    virtual void draw_text(litehtml::uint_ptr hdc,
        const litehtml::tchar_t* text,
        litehtml::uint_ptr hFont,
        litehtml::web_color color,
        const litehtml::position& pos) override;

    virtual int pt_to_px(int pt) override;

    virtual int get_default_font_size() const override;

    virtual const litehtml::tchar_t* get_default_font_name() const override;

    virtual void load_image(const litehtml::URL& src,
        bool redraw_on_ready) override;

    virtual void get_image_size(const litehtml::tchar_t* src,
        const litehtml::tchar_t* baseurl,
        litehtml::size& sz) override;

    virtual litehtml::size get_image_size(const litehtml::URL& src) override;

    virtual void draw_background(litehtml::uint_ptr hdc,
        const litehtml::background_paint& bg) override;

    virtual void draw_borders(litehtml::uint_ptr hdc,
        const litehtml::borders& borders,
        const litehtml::position& draw_pos,
        bool root) override;

    virtual void draw_list_marker(litehtml::uint_ptr hdc,
        const litehtml::list_marker& marker) override;

    virtual std::shared_ptr<litehtml::element> create_element(
        const litehtml::tchar_t* tag_name,
        const litehtml::string_map& attributes,
        const std::shared_ptr<litehtml::document>& doc) override;

    virtual void get_media_features(litehtml::media_features& media) const override;

    virtual void get_language(litehtml::tstring& language,
        litehtml::tstring& culture) const override;

    virtual void link(const std::shared_ptr<litehtml::document>& ptr,
        const litehtml::element::ptr& el) override;

    virtual void transform_text(litehtml::tstring& text,
        litehtml::text_transform tt) override;

    virtual void set_clip(const litehtml::position& pos,
        const litehtml::border_radiuses& bdr_radius,
        bool valid_x,
        bool valid_y) override;

    virtual void del_clip() override;

    virtual void make_url(const litehtml::tchar_t* url,
        const litehtml::tchar_t* basepath,
        litehtml::tstring& out);

    virtual void set_caption(const litehtml::tchar_t* caption) override;

    virtual void on_anchor_click(const litehtml::tchar_t* url,
        const litehtml::element::ptr& el) override;

    virtual void set_cursor(const litehtml::tchar_t* cursor) override;

    virtual void import_css(litehtml::tstring& text,
        const litehtml::tstring& url,
        litehtml::tstring& baseurl) override;

    virtual litehtml::tstring import_css(const litehtml::URL& url) override;

    virtual void get_client_rect(litehtml::position& client) const override;
};

#endif // TEST_TEST_CONTAINER_H__