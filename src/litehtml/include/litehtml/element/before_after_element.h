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

#ifndef LITEHTML_BEFORE_AFTER_ELEMENT_H__
#define LITEHTML_BEFORE_AFTER_ELEMENT_H__

#include "litehtml/element/html_element.h"

namespace litehtml {

class BeforeAfterBaseElement : public HTMLElement {
public:
    BeforeAfterBaseElement(Document* doc, bool before);
    virtual ~BeforeAfterBaseElement() override;

    virtual void add_style(const CSSStyle& st) override;
    virtual void apply_stylesheet(
        const litehtml::CSSStylesheet& stylesheet) override;

private:
    void add_text(const std::string& txt);
    void add_function(const std::string& fnc, const std::string& params);
    char convert_escape(const char* txt);
};

class BeforeElement : public BeforeAfterBaseElement {
public:
    BeforeElement(Document* doc)
    : BeforeAfterBaseElement(doc, true)
    {
    }

    virtual ~BeforeElement();

    virtual ElementType type() const override
    {
        return kElementBefore;
    }

};

class AfterElement : public BeforeAfterBaseElement {
public:
    AfterElement(Document* doc)
    : BeforeAfterBaseElement(doc, false)
    {
    }

    virtual ElementType type() const override
    {
        return kElementAfter;
    }

    virtual ~AfterElement();
};

} // namespace litehtml

#endif // LITEHTML_BEFORE_AFTER_ELEMENT_H__
