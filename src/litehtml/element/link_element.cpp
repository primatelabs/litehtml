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

#include "litehtml/element/link_element.h"

#include "litehtml/document.h"
#include "litehtml/document_container.h"

namespace litehtml {

LinkElement::LinkElement(Document* doc)
: HTMLElement(doc)
{
}

LinkElement::~LinkElement()
{
}

void LinkElement::parse_attributes()
{
    bool processed = false;

    Document* doc = get_document();
    DocumentContainer* ctr = doc->container();

    const char* rel = get_attr("rel");
    if (rel && !strcmp(rel, "stylesheet")) {
        const char* media = get_attr("media");
        const char* href = get_attr("href");
        if (href && href[0]) {
            URL css_url = resolve(doc->base_url(), URL(href));
            std::string css_text = ctr->import_css(css_url);
            if (!css_text.empty()) {
                doc->add_stylesheet(css_text, css_url, media);
                processed = true;
            }
        }
    }

    if (!processed) {
        doc->container()->link(doc, this);
    }
}

} // namespace litehtml
