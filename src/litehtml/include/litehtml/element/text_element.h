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

#ifndef LITEHTML_TEXT_ELEMENT_H__
#define LITEHTML_TEXT_ELEMENT_H__

#include "litehtml/css/css_property.h"
#include "litehtml/element/html_element.h"

namespace litehtml {

class TextElement : public Element {
protected:
    std::string text_;

    std::string transformed_text_;

    Size size_;

    TextTransform text_transform_ = kTextTransformNone;

    bool use_transformed_ = false;

    bool draw_spaces_ = true;

public:
    TextElement() = delete;

    explicit TextElement(Document* document);

    TextElement(Document* document, const char* text);

    TextElement(Document* document, const char* text, size_t length);

    virtual ~TextElement() override;

    virtual ElementType type() const override
    {
        return kElementText;
    }

    virtual void get_text(std::string& text) const override;

    virtual const char* get_style_property(CSSProperty name) override;

    virtual void parse_styles(bool is_reparse) override;
    virtual int get_baseline() override;
    virtual void draw(uintptr_t hdc, int x, int y, const Position* clip) override;
    virtual int line_height() const override;
    virtual uintptr_t get_font(FontMetrics* fm = nullptr) override;
    virtual Display get_display() const override;
    virtual WhiteSpace get_white_space() const override;
    virtual ElementPosition get_element_position(
        CSSOffsets* offsets = nullptr) const override;
    virtual CSSOffsets get_css_offsets() const override;

protected:
    virtual Size get_content_size(int max_width) override;
};

} // namespace litehtml

#endif // LITEHTML_TEXT_ELEMENT_H__
