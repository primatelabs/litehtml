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

#ifndef LITEHTML_MEDIA_QUERY_EXPRESSION_H__
#define LITEHTML_MEDIA_QUERY_EXPRESSION_H__

#include <vector>

#include "litehtml/media_features.h"

namespace litehtml {

struct MediaQueryExpression {

    MediaFeature feature = kMediaFeatureNone;

    // From https://www.w3.org/TR/css3-mediaqueries/#media1:
    //
    //      Media features only accept single values: one keyword, one number,
    //      or a number with a unit identifier. (The only exceptions are the
    //      ‘aspect-ratio’ and ‘device-aspect-ratio’ media features.)
    //
    // TODO: Find a more elegant way to store values.

    int val = 0;

    int val2 = 0;

    // TODO: Determine when features are "checked as boolean" (i.e., the value
    // of the feature is treateda as a boolean where 0 indicates false and all
    // other values indicate true).

    bool check_as_bool = false;

    MediaQueryExpression() = default;

    MediaQueryExpression(MediaFeature feature, int value)
    : feature(feature)
    , val(value)
    {
    }

    MediaQueryExpression(MediaFeature feature, int value1, int value2)
    : feature(feature)
    , val(value1)
    , val2(value2)
    {
    }

    bool check(const MediaFeatures& features) const;
};

} // namespace litehtml

#endif // LITEHTML_MEDIA_QUERY_EXPRESSION_H__