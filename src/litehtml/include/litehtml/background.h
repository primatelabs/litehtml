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

#ifndef LITEHTML_BACKGROUND_H__
#define LITEHTML_BACKGROUND_H__

#include "litehtml/borders.h"
#include "litehtml/css/css_length.h"
#include "litehtml/css/css_position.h"
#include "litehtml/types.h"
#include "litehtml/url.h"
#include "litehtml/web_color.h"

namespace litehtml {

#define BACKGROUND_ATTACHMENT_STRINGS _t("scroll;fixed")

enum BackgroundAttachment {
    kBackgroundAttachmentScroll,
    kBackgroundAttachmentFixed
};

#define BACKGROUND_REPEAT_STRINGS _t("repeat;repeat-x;repeat-y;no-repeat")

enum BackgroundRepeat {
    kBackgroundRepeatRepeat,
    kBackgroundRepeatRepeatX,
    kBackgroundRepeatRepeatY,
    kBackgroundRepeatNoRepeat
};

#define BACKGROUND_BOX_STRINGS _t("border-box;padding-box;content-box")

enum BackgroundBox {
    kBackgroundBoxBorderBox,
    kBackgroundBoxPaddingBox,
    kBackgroundBoxContentBox
};

class Background {
public:
    URL m_image;
    WebColor m_color;
    BackgroundAttachment m_attachment;
    CSSPosition m_position;
    BackgroundRepeat m_repeat;
    BackgroundBox m_clip;
    BackgroundBox m_origin;
    css_border_radius m_radius;

public:
    Background(void);
    Background(const Background& val);
    ~Background(void);

    Background& operator=(const Background& val);
};

} // namespace litehtml

#endif // LITEHTML_BACKGROUND_H__