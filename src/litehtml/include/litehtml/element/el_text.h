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

#ifndef LITEHTML_EL_TEXT_H__
#define LITEHTML_EL_TEXT_H__
#include "html_tag.h"

namespace litehtml {
class el_text : public element {
protected:
    tstring m_text;
    tstring m_transformed_text;
    size m_size;
    text_transform m_text_transform;
    bool m_use_transformed;
    bool m_draw_spaces;

public:
    el_text(const tchar_t* text, const std::shared_ptr<litehtml::document>& doc);
    virtual ~el_text();

    virtual void get_text(tstring& text) override;
    virtual const tchar_t* get_style_property(const tchar_t* name,
        bool inherited,
        const tchar_t* def = 0) override;
    virtual void parse_styles(bool is_reparse) override;
    virtual int get_base_line() override;
    virtual void draw(uint_ptr hdc, int x, int y, const position* clip) override;
    virtual int line_height() const override;
    virtual uint_ptr get_font(font_metrics* fm = 0) override;
    virtual style_display get_display() const override;
    virtual white_space get_white_space() const override;
    virtual element_position get_element_position(
        css_offsets* offsets = 0) const override;
    virtual css_offsets get_css_offsets() const override;

protected:
    virtual void get_content_size(size& sz, int max_width) override;
};
} // namespace litehtml

#endif // LITEHTML_EL_TEXT_H__