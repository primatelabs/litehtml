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

#ifndef LITEHTML_BACKGROUND_H__
#define LITEHTML_BACKGROUND_H__
#include "attributes.h"
#include "borders.h"
#include "css_length.h"
#include "css_position.h"
#include "types.h"
#include "web_color.h"

namespace litehtml {
class background {
public:
    tstring m_image;
    tstring m_baseurl;
    web_color m_color;
    background_attachment m_attachment;
    css_position m_position;
    background_repeat m_repeat;
    background_box m_clip;
    background_box m_origin;
    css_border_radius m_radius;

public:
    background(void);
    background(const background& val);
    ~background(void);

    background& operator=(const background& val);
};

class background_paint {
public:
    tstring image;
    tstring baseurl;
    background_attachment attachment;
    background_repeat repeat;
    web_color color;
    position clip_box;
    position origin_box;
    position border_box;
    border_radiuses border_radius;
    size image_size;
    int position_x;
    int position_y;
    bool is_root;

public:
    background_paint();
    background_paint(const background_paint& val);
    void operator=(const background& val);
};

} // namespace litehtml

#endif // LITEHTML_BACKGROUND_H__