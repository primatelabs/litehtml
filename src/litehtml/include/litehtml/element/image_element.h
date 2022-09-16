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

#ifndef LITEHTML_IMAGE_ELEMENT_H__
#define LITEHTML_IMAGE_ELEMENT_H__

#include "litehtml/element/html_element.h"

namespace litehtml {

class ImageElement : public HTMLElement {
    URL src_;

public:
    ImageElement(Document* doc);
    virtual ~ImageElement(void) override;

    virtual ElementType type() const override
    {
        return kElementImage;
    }

    virtual int line_height() const override;
    virtual bool is_replaced() const override;
    virtual int render(int x, int y, int max_width, bool second_pass = false) override;
    virtual void parse_attributes() override;
    virtual void parse_styles(bool is_reparse = false) override;
    virtual void draw(uintptr_t hdc, int x, int y, const Position* clip) override;
    virtual Size get_content_size(int max_width) override;

private:
    int calc_max_height(int image_height);
};
} // namespace litehtml

#endif // LITEHTML_IMAGE_ELEMENT_H__
