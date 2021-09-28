// Copyright (C) 2020-2021 Primate Labs Inc. All rights reserved.
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
//    * Neither the name of the copyright holder nor the names of its
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

#include <fstream>
#include <iostream>
#include <string>

#include "headless_container.h"
#include "litehtml/litehtml.h"

namespace {

const char* master_stylesheet = {
#include "master.css.inc"
};

std::string load(const std::string& filename)
{
    std::ifstream ifs(filename);

    if (ifs.bad()) {
        exit(-1);
    }

    std::string data;
    char c;
    // TODO: Is there a better way to load a file into memory?
    while (ifs.get(c)) {
        data += c;
    }

    return data;
}

} // namespace

int main(int argc, char** argv)
{
    std::string html = load(argv[1]);

    litehtml::context ctx;
    ctx.load_master_stylesheet(master_stylesheet);

    headless_container container;
    litehtml::document::ptr doc =
        litehtml::document::createFromString(html.c_str(), &container, &ctx);

    doc->render(1000);

    cairo_surface_t* surface =
        cairo_image_surface_create(CAIRO_FORMAT_RGB24, doc->width(), doc->height());
    cairo_t* cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    doc->draw((litehtml::uint_ptr)cr, 0, 0, nullptr);
    cairo_surface_write_to_png(surface, "headless.png");

    return 0;
}
