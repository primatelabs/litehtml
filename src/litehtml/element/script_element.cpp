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

#include "litehtml/element/script_element.h"

#include "litehtml/document.h"
#include "litehtml/document_container.h"

namespace litehtml {

ScriptElement::ScriptElement(Document* doc)
: Element(doc)
{
}

ScriptElement::~ScriptElement()
{
}

void ScriptElement::set_attr(const char* name, const char* val)
{
    if (name && val) {
        std::string s_val = name;
        for (size_t i = 0; i < s_val.length(); i++) {
            s_val[i] = tolower(s_val[i]);
        }
        m_attrs[s_val] = val;
    }
}

const char* ScriptElement::get_attr(const char* name, const char* def) const
{
    string_map::const_iterator attr = m_attrs.find(name);
    if (attr != m_attrs.end()) {
        return attr->second.c_str();
    }
    return def;
}

void ScriptElement::parse_attributes()
{
    // TODO: pass script text to document container
    Document* doc = get_document();
    DocumentContainer* ctr = doc->container();

    const char* src = get_attr("src");
    if (src && src[0]) {
        URL js_url = resolve(doc->base_url(), URL(src));
        std::string js_text = ctr->import_js(js_url);
        if (!js_text.empty()) {
            doc->add_javascript(js_text, js_url);
        }
    }
}

bool ScriptElement::append_child(Element* element)
{
    element->get_text(m_text);
    return false;
}

const char* ScriptElement::get_tagName() const
{
    return "script";
}

} // namespace litehtml
