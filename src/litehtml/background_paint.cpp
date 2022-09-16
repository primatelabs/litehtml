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

#include "litehtml/background_paint.h"

#include "litehtml/html.h"

namespace litehtml {

BackgroundPaint::BackgroundPaint()
: color(0, 0, 0, 0)
{
    position_x = 0;
    position_y = 0;
    attachment = kBackgroundAttachmentScroll;
    repeat = kBackgroundRepeatRepeat;
    is_root = false;
}

BackgroundPaint::BackgroundPaint(const BackgroundPaint& val)
{
    image = val.image;
    attachment = val.attachment;
    repeat = val.repeat;
    color = val.color;
    clip_box = val.clip_box;
    origin_box = val.origin_box;
    border_box = val.border_box;
    border_radii = val.border_radii;
    image_size = val.image_size;
    position_x = val.position_x;
    position_y = val.position_y;
    is_root = val.is_root;
}

void BackgroundPaint::operator=(const Background& val)
{
    attachment = val.m_attachment;
    image = val.m_image;
    repeat = val.m_repeat;
    color = val.m_color;
}

} // namespace litehtml
