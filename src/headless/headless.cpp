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

#include "flags.h"
#include "headless_container.h"
#include "http.h"
#include "litehtml/document_parser.h"
#include "litehtml/litehtml.h"
#include "litehtml/logging.h"
#include "image/png_codec.h"
#include "orion_render_context.h"

using namespace headless;
using namespace litehtml;

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
    Flags flags;

    if (flags.parse(argc, argv) != kParseSuccess) {
        flags.usage(-1);
    }

    if (flags.do_usage) {
        flags.usage(0);
    }

    if (flags.url.empty() && flags.file.empty()) {
        flags.usage(-1);
    }

    litehtml::URL url(flags.url);

    std::string html;

    if (!flags.file.empty()) {
        html = load(url.path());
    } else {
        http_response response = http_request(url);
        html = response.body;
    }

    litehtml::Context ctx(master_stylesheet);

    HeadlessContainer container;

    std::unique_ptr<Document> document(DocumentParser::parse(
        html,
        url,
        &container,
        &ctx));

    document->render(1800);

    OrionRenderContext orc(document->width(), document->height());

    orion::rasterizer_scanline_aa<> ras;
    orion::scanline_p8 scanline;
    ras.auto_close(false);

    document->draw(reinterpret_cast<uintptr_t>(&orc), 0, 0, nullptr);
    orc.canvas.save<PNGCodec>(flags.output);

#if defined(ENABLE_JSON)
    std::ofstream ofs_stylesheet("stylesheet.json");
    ofs_stylesheet << document->stylesheet().json().dump(2);

    std::ofstream ofs_document("document.json");
    ofs_document << document->json().dump(2);
#endif

    return 0;
}
