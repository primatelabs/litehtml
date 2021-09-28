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

#include "test_container.h"

test_container::test_container()
{
}

test_container::~test_container()
{
}

litehtml::uint_ptr test_container::create_font(const litehtml::tchar_t* faceName,
    int size,
    int weight,
    litehtml::font_style italic,
    unsigned int decoration,
    litehtml::font_metrics* fm)
{
    if (fm) {
        fm->ascent = 10;
        fm->descent = 5;
        fm->height = 10 + 5;
        fm->x_height = 3;
    }
    return (litehtml::uint_ptr)0;
}

void test_container::delete_font(litehtml::uint_ptr hFont)
{
}

int test_container::text_width(const litehtml::tchar_t* text,
    litehtml::uint_ptr hFont)
{
    return 0;
}

void test_container::draw_text(litehtml::uint_ptr hdc,
    const litehtml::tchar_t* text,
    litehtml::uint_ptr hFont,
    litehtml::web_color color,
    const litehtml::position& pos)
{
}

int test_container::pt_to_px(int pt)
{
    return (int)((double)pt * 96 / 72.0);
}

int test_container::get_default_font_size() const
{
    return 16;
}

const litehtml::tchar_t* test_container::get_default_font_name() const
{
    return _t("Times New Roman");
}

void test_container::draw_list_marker(litehtml::uint_ptr hdc,
    const litehtml::list_marker& marker)
{
}

void test_container::load_image(const litehtml::tchar_t* src,
    const litehtml::tchar_t* baseurl,
    bool redraw_on_ready)
{
}

void test_container::get_image_size(const litehtml::tchar_t* src,
    const litehtml::tchar_t* baseurl,
    litehtml::size& sz)
{
}

void test_container::draw_background(litehtml::uint_ptr hdc,
    const litehtml::background_paint& bg)
{
}

void test_container::make_url(const litehtml::tchar_t* url,
    const litehtml::tchar_t* basepath,
    litehtml::tstring& out)
{
    out = url;
}

void test_container::draw_borders(litehtml::uint_ptr hdc,
    const litehtml::borders& borders,
    const litehtml::position& draw_pos,
    bool root)
{
}

void test_container::set_caption(const litehtml::tchar_t* caption)
{
}

void test_container::set_base_url(const litehtml::tchar_t* base_url)
{
}

void test_container::link(const std::shared_ptr<litehtml::document>& ptr,
    const litehtml::element::ptr& el)
{
}
void test_container::on_anchor_click(const litehtml::tchar_t* url,
    const litehtml::element::ptr& el)
{
}

void test_container::set_cursor(const litehtml::tchar_t* cursor)
{
}

void test_container::transform_text(litehtml::tstring& text,
    litehtml::text_transform tt)
{
}

void test_container::import_css(litehtml::tstring& text,
    const litehtml::tstring& url,
    litehtml::tstring& baseurl)
{
}

void test_container::set_clip(const litehtml::position& pos,
    const litehtml::border_radiuses& bdr_radius,
    bool valid_x,
    bool valid_y)
{
}

void test_container::del_clip()
{
}

void test_container::get_client_rect(litehtml::position& client) const
{
}

std::shared_ptr<litehtml::element> test_container::create_element(
    const litehtml::tchar_t* tag_name,
    const litehtml::string_map& attributes,
    const std::shared_ptr<litehtml::document>& doc)
{
    return 0;
}

void test_container::get_media_features(litehtml::media_features& media) const
{
    litehtml::position client;
    get_client_rect(client);
    media.type = litehtml::media_type_screen;
    media.width = client.width;
    media.height = client.height;
    media.device_width = 100;
    media.device_height = 100;
    media.color = 8;
    media.monochrome = 0;
    media.color_index = 256;
    media.resolution = 96;
}

void test_container::get_language(litehtml::tstring& language,
    litehtml::tstring& culture) const
{
    language = _t("en");
    culture = _t("");
}
