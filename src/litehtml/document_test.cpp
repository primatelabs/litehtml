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

#include "litehtml/document.h"

#include <gtest/gtest.h>

#include "litehtml/document_parser.h"
#include "test_container.h"

using namespace litehtml;

TEST(DocumentTest, AddFont)
{
    test_container container;
    Document* doc = new Document(&container, nullptr);
    FontMetrics fm;
    doc->get_font(nullptr, 0, "normal", "normal", "", &fm);
    doc->get_font("inherit", 0, "normal", "normal", "", &fm);
    doc->get_font("Arial", 0, "bold", "normal", "underline", &fm);
    doc->get_font("Arial", 0, "bold", "normal", "line-through", &fm);
    doc->get_font("Arial", 0, "bold", "normal", "overline", &fm);
}

TEST(DocumentTest, Render)
{
    Context context;
    test_container container;
    Document* document = DocumentParser::parse("<html>Body</html>",
        URL(),
        &container,
        &context);
    document->render(100);
}

TEST(DocumentTest, Draw)
{
    Context context;
    test_container container;
    Document* document = DocumentParser::parse("<html>Body</html>",
        URL(),
        &container,
        &context);
    Position position(0, 0, 100, 100);
    document->draw((uintptr_t)0, 0, 0, &position);
}

TEST(DocumentTest, CvtUnits)
{
    test_container container;
    Document* doc = new Document(&container, nullptr);
    bool is_percent;
    doc->cvt_units("", 10, &is_percent);
    CSSLength c;
    c.parse_length_string("10%"), doc->cvt_units(c, 10, 100);
    c.parse_length_string("10em"), doc->cvt_units(c, 10, 100);
    c.parse_length_string("10pt"), doc->cvt_units(c, 10, 100);
    c.parse_length_string("10in"), doc->cvt_units(c, 10, 100);
    c.parse_length_string("10cm"), doc->cvt_units(c, 10, 100);
    c.parse_length_string("10mm"), doc->cvt_units(c, 10, 100);
    c.parse_length_string("10vm"), doc->cvt_units(c, 10, 100);
    c.parse_length_string("10vh"), doc->cvt_units(c, 10, 100);
    c.parse_length_string("10vmin"), doc->cvt_units(c, 10, 100);
    c.parse_length_string("10vmax"), doc->cvt_units(c, 10, 100);
    c.parse_length_string("10"), doc->cvt_units(c, 10, 100);
}

TEST(DocumentTest, MouseEvents)
{
    test_container container;
    Document* doc = new Document(&container, nullptr);
    std::vector<Position> redraw_boxes;
    doc->on_mouse_over(0, 0, 0, 0, redraw_boxes);
    doc->on_lbutton_down(0, 0, 0, 0, redraw_boxes);
    doc->on_lbutton_up(0, 0, 0, 0, redraw_boxes);
    doc->on_mouse_leave(redraw_boxes);
}

TEST(DocumentTest, CreateElement)
{
    test_container container;
    Document* doc = new Document(&container, nullptr);
    string_map map;
    doc->create_element("container", map);
    doc->create_element("br", map);
    doc->create_element("p", map);
    doc->create_element("img", map);
    doc->create_element("table", map);
    doc->create_element("td", map);
    doc->create_element("th", map);
    doc->create_element("link", map);
    doc->create_element("title", map);
    doc->create_element("a", map);
    doc->create_element("tr", map);
    doc->create_element("style", map);
    doc->create_element("base", map);
    doc->create_element("div", map);
    doc->create_element("script", map);
    doc->create_element("font", map);
    doc->create_element("tag", map);
}

TEST(DocumentTest, DeviceChange)
{
    test_container container;
    Document* doc = new Document(&container, nullptr);
    doc->media_changed();
    doc->lang_changed();
}
