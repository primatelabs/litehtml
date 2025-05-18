// Copyright (c) 2013, Yuri Kobets (tordex)
// Copyright (c) 2020-2021 Primate Labs Inc.
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

#include "litehtml/media_query_list.h"

#include "litehtml/document.h"
#include "litehtml/document_container.h"
#include "litehtml/html.h"

namespace litehtml {

MediaQueryList::ptr MediaQueryList::create_from_string(const std::string& str,
    const Document* doc)
{
    MediaQueryList::ptr list = std::make_shared<MediaQueryList>();

    string_vector tokens;
    split_string(str, tokens, ",");

    for (string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++) {
        trim(*tok);
        lcase(*tok);

        MediaQuery::ptr query = MediaQuery::create_from_string(*tok, doc);
        if (query) {
            list->queries_.push_back(query);
        }
    }
    if (list->queries_.empty()) {
        list = nullptr;
    }

    return list;
}

bool MediaQueryList::apply_MediaFeatures(const MediaFeatures& features)
{
    bool apply = false;

    for (MediaQuery::vector::iterator iter = queries_.begin();
         iter != queries_.end() && !apply;
         iter++) {
        if ((*iter)->check(features)) {
            apply = true;
        }
    }

    bool ret = (apply != is_used_);
    is_used_ = apply;
    return ret;
}

} // namespace litehtml
