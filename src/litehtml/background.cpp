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

#include "litehtml/background.h"

#include "litehtml/html.h"

namespace litehtml {

Background::Background(void)
{
    m_attachment = kBackgroundAttachmentScroll;
    m_repeat = kBackgroundRepeatRepeat;
    m_clip = kBackgroundBoxBorderBox;
    m_origin = kBackgroundBoxPaddingBox;
    m_color.alpha = 0;
    m_color.red = 0;
    m_color.green = 0;
    m_color.blue = 0;
}

Background::Background(const Background& val)
{
    m_image = val.m_image;
    m_color = val.m_color;
    m_attachment = val.m_attachment;
    m_position = val.m_position;
    m_repeat = val.m_repeat;
    m_clip = val.m_clip;
    m_origin = val.m_origin;
}

Background::~Background(void)
{
}

Background& Background::operator=(const Background& val)
{
    m_image = val.m_image;
    m_color = val.m_color;
    m_attachment = val.m_attachment;
    m_position = val.m_position;
    m_repeat = val.m_repeat;
    m_clip = val.m_clip;
    m_origin = val.m_origin;
    return *this;
}

} // namespace litehtml
