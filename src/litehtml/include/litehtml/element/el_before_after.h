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

#ifndef LITEHTML_EL_BEFORE_AFTER_H__
#define LITEHTML_EL_BEFORE_AFTER_H__
#include "html_tag.h"

namespace litehtml {
class el_before_after_base : public html_tag {
public:
    el_before_after_base(const std::shared_ptr<litehtml::document>& doc,
        bool before);
    virtual ~el_before_after_base() override;

    virtual void add_style(const litehtml::style& st) override;
    virtual void apply_stylesheet(
        const litehtml::css_stylesheet& stylesheet) override;

private:
    void add_text(const tstring& txt);
    void add_function(const tstring& fnc, const tstring& params);
    tchar_t convert_escape(const tchar_t* txt);
};

class el_before : public el_before_after_base {
public:
    el_before(const std::shared_ptr<litehtml::document>& doc)
    : el_before_after_base(doc, true)
    {
    }

    virtual ~el_before();
};

class el_after : public el_before_after_base {
public:
    el_after(const std::shared_ptr<litehtml::document>& doc)
    : el_before_after_base(doc, false)
    {
    }

    virtual ~el_after();
};
} // namespace litehtml

#endif // LITEHTML_EL_BEFORE_AFTER_H__