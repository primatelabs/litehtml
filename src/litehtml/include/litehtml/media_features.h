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

#ifndef LITEHTML_MEDIA_FEATURES_H__
#define LITEHTML_MEDIA_FEATURES_H__

#include <stdlib.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "litehtml/string.h"

namespace litehtml {

#define MEDIA_ORIENTATION_STRINGS _t("portrait;landscape")

enum MediaOrientation {
    kMediaOrientationPortrait,
    kMediaOrientationLandscape,
};

#define MEDIA_FEATURE_STRINGS                                                 \
    _t("none;width;min-width;max-width;height;min-height;max-height;device-"  \
       "width;min-device-width;max-device-width;device-height;min-device-"    \
       "height;max-device-height;orientation;aspect-ratio;min-aspect-ratio;"  \
       "max-aspect-ratio;device-aspect-ratio;min-device-aspect-ratio;max-"    \
       "device-aspect-ratio;color;min-color;max-color;color-index;min-color-" \
       "index;max-color-index;monochrome;min-monochrome;max-monochrome;"      \
       "resolution;min-resolution;max-resolution")

enum MediaFeature {
    kMediaFeatureNone,

    kMediaFeatureWidth,
    kMediaFeatureMinWidth,
    kMediaFeatureMaxWidth,

    kMediaFeatureHeight,
    kMediaFeatureMinHeight,
    kMediaFeatureMaxHeight,

    kMediaFeatureDeviceWidth,
    kMediaFeatureMinDeviceWidth,
    kMediaFeatureMaxDeviceWidth,

    kMediaFeatureDeviceHeight,
    kMediaFeatureMinDeviceHeight,
    kMediaFeatureMaxDeviceHeight,

    kMediaFeatureOrientation,

    kMediaFeatureAspectRatio,
    kMediaFeatureMinAspectRatio,
    kMediaFeatureMaxAspectRatio,

    kMediaFeatureDeviceAspectRatio,
    kMediaFeatureMinDeviceAspectRatio,
    kMediaFeatureMaxDeviceAspectRatio,

    kMediaFeatureColor,
    kMediaFeatureMinColor,
    kMediaFeatureMaxColor,

    kMediaFeatureColorIndex,
    kMediaFeatureMinColorIndex,
    kMediaFeatureMaxColorIndex,

    kMediaFeatureMonochrome,
    kMediaFeatureMinMonochrome,
    kMediaFeatureMaxMonochrome,

    kMediaFeatureResolution,
    kMediaFeatureMinResolution,
    kMediaFeatureMaxResolution,
};

#define MEDIA_TYPE_STRINGS                                                  \
    _t("none;all;screen;print;braille;embossed;handheld;projection;speech;" \
       "tty;tv")

enum MediaType {
    kMediaTypeNone,
    kMediaTypeAll,
    kMediaTypeScreen,
    kMediaTypePrint,
    kMediaTypeBraille,
    kMediaTypeEmbossed,
    kMediaTypeHandheld,
    kMediaTypeProjection,
    kMediaTypeSpeech,
    kMediaTypeTTY,
    kMediaTypeTV,
};

struct MediaFeatures {
    MediaType type = kMediaTypeNone;

    // The width of the targeted display area of the output in pixels.  For
    // continuous media, this is the width of the viewport including the size
    // of a rendered scroll bar (if any). For paged media, this is the width
    // of the page box.
    int width = 0;

    // The height of the targeted display area of the output in pixels.  For
    // continuous media, this is the height of the viewport including the size
    // of a rendered scroll bar (if any). For paged media, this is the height
    // of the page box.
    int height = 0;

    // The width of the rendering surface of the output device in pixels.  For
    // continuous media, this is the width of the screen.  For paged media,
    // this is the width of the page sheet.
    int device_width = 0;

    // The height of the rendering surface of the output device in pixels.  For
    // continuous media, this is the height of the screen.  For paged media,
    // this is the height of the page sheet.
    int device_height = 0;

    // The number of bits per color component of the output device, or zero if
    // the device is not a color device.
    int color = 0;

    // The number of entries in the color lookup table of the output device,
    // or zero if the device does not use a color lookup table.
    int color_index = 0;

    // The number of bits per pixel of the output device, of zero if the
    // device is not a monochrome device.
    int monochrome = 0;

    // The resolution of the output device in DPI.
    int resolution = 0;
};

} // namespace litehtml

#endif // LITEHTML_MEDIA_FEATURES_H__
