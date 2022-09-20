// Copyright (c) 2013, Yuri Kobets (tordex)
// Copyright (C) 2020-2022 Primate Labs Inc.
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

#include "litehtml/document_parser.h"

#include <gumbo.h>

#include "litehtml/document_container.h"

namespace litehtml {

Document* DocumentParser::parse(const String& html,
    const URL& base_url,
    DocumentContainer* container,
    Context* context,
    CSSStylesheet* user_stylesheet)
{
    return parse(html.data(),
        html.length(),
        base_url,
        container,
        context,
        user_stylesheet);
}

Document* DocumentParser::parse(const char* html,
    const URL& base_url,
    DocumentContainer* container,
    Context* context,
    CSSStylesheet* user_stylesheet)
{
    return parse(html,
        strlen(html),
        base_url,
        container,
        context,
        user_stylesheet);
}

Document* DocumentParser::parse(const char* html,
    size_t length,
    const URL& base_url,
    DocumentContainer* container,
    Context* context,
    CSSStylesheet* user_stylesheet)
{
    // Parse the HTML using gumbo_parse_with_options() with the default
    // options. We're only using gumbo_parse_with_options() here so we can
    // pass in the length of the string.
    GumboOutput* output = gumbo_parse_with_options(&kGumboDefaultOptions,
        html,
        length);

    // Create the document.
    Document* document = new Document(base_url, container, context);

    // Convert the Gumbo elements into litehtml elements.
    ElementsVector root_elements;
    document->create_node(output->root, root_elements, true);
    if (!root_elements.empty()) {
        assert(root_elements.size() == 1);
        document->root_.reset(root_elements.back());
    }

    // We no longer need the Gumbo output (litehtml elements do not reference
    // memory from Gumbo elements). Destroy the Gumbo output.
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    // Process the litehtml elements. This includes parsing stylesheets,
    // applying stylesheets to the document, and fixing up table elements
    // (if necessary).
    if (document->root_) {
        document->container()->get_media_features(document->m_media);

        // Apply the master (agent?) stylesheet.
        document->root_->apply_stylesheet(context->master_stylesheet());

        // Parse element attributes.
        document->root_->parse_attributes();

        // Parse stylesheets linked from document.
        MediaQueryList::ptr media = nullptr;
        for (auto& css : document->m_css) {
            if (!css.media.empty()) {
                media = MediaQueryList::create_from_string(css.media, document);
            }

            document->stylesheet_.parse(css.text, css.baseurl, document, media);
        }
        for (css_text::vector::iterator css = document->m_css.begin();
             css != document->m_css.end();
             css++) {
            if (!css->media.empty()) {
                media = MediaQueryList::create_from_string(css->media, document);
            } else {
                media = nullptr;
            }
            document->stylesheet_.parse(css->text,
                css->baseurl,
                document,
                media);
        }

        // Sort CSS selectors using CSS rules.
        document->stylesheet_.sort_selectors();

        // Get the current media features for the document.
        if (!document->m_media_lists.empty()) {
            document->update_media_lists(document->m_media);
        }

        // Apply parsed CSS styles.
        document->root_->apply_stylesheet(document->stylesheet_);

        // Apply the user stylesheet (if provided).
        if (user_stylesheet) {
            document->root_->apply_stylesheet(*user_stylesheet);
        }

        // Parse applied styles in the elements.
        document->root_->parse_styles();

        // Now the m_tabular_elements is filled with tabular elements.
        // We have to check the tabular elements for missing table elements
        // and create the anonymous boxes in visual table layout
        document->fix_tables_layout();

        // Finally initialize all of the elements.
        //
        // FIXME: Consider renaming this function as it is only implemented by
        // HTMLElement and HTMLElement::init() only sets up the table grid
        // (which is why this call appears after fix_tables_layout()).
        document->root_->init();
    }

    return document;
}

} // namespace litehtml
