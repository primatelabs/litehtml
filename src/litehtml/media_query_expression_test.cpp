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

#include "litehtml/media_query_expression.h"

#include <gtest/gtest.h>

using namespace litehtml;

TEST(MediaQueryExpressionTest, Check)
{
    MediaQueryExpression e;
    MediaFeatures k;

    e = MediaQueryExpression(kMediaFeatureWidth, 100);
    k = MediaFeatures(), k.width = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.width = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.width = 500;
    EXPECT_FALSE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMinWidth, 100);
    k = MediaFeatures(), k.width = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.width = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.width = 500;
    EXPECT_TRUE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMaxWidth, 100);
    k = MediaFeatures(), k.width = 0;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.width = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.width = 500;
    EXPECT_FALSE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureHeight, 100);
    k = MediaFeatures(), k.height = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.height = 500;
    EXPECT_FALSE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMinHeight, 100);
    k = MediaFeatures(), k.height = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.height = 500;
    EXPECT_TRUE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMaxHeight, 100);
    k = MediaFeatures(), k.height = 0;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.height = 500;
    EXPECT_FALSE(e.check(k));

    e = MediaQueryExpression(kMediaFeatureDeviceWidth, 100);
    k = MediaFeatures(), k.device_width = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.device_width = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.device_width = 500;
    EXPECT_FALSE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMinDeviceWidth, 100);
    k = MediaFeatures(), k.device_width = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.device_width = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.device_width = 500;
    EXPECT_TRUE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMaxDeviceWidth, 100);
    k = MediaFeatures(), k.device_width = 0;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.device_width = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.device_width = 500;
    EXPECT_FALSE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureDeviceHeight, 100);
    k = MediaFeatures(), k.device_height = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.device_height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.device_height = 500;
    EXPECT_FALSE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMinDeviceHeight, 100);
    k = MediaFeatures(), k.device_height = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.device_height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.device_height = 500;
    EXPECT_TRUE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMaxDeviceHeight, 100);
    k = MediaFeatures(), k.device_height = 0;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.device_height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.device_height = 500;
    EXPECT_FALSE(e.check(k));

    e = MediaQueryExpression(kMediaFeatureOrientation, (int)kMediaOrientationPortrait);
    k = MediaFeatures(), k.width = 0, k.height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.width = 100, k.height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.width = 500, k.height = 100;
    EXPECT_FALSE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureOrientation, (int)kMediaOrientationLandscape);
    k = MediaFeatures(), k.width = 0, k.height = 100;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.width = 100, k.height = 100;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.width = 500, k.height = 100;
    EXPECT_TRUE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureAspectRatio, 100, 100);
    k = MediaFeatures(), k.width = 0, k.height = 100;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.width = 100, k.height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.width = 500, k.height = 100;
    EXPECT_FALSE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMinAspectRatio, 100, 100);
    k = MediaFeatures(), k.width = 0, k.height = 100;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.width = 100, k.height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.width = 500, k.height = 100;
    EXPECT_TRUE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMaxAspectRatio, 100, 100);
    k = MediaFeatures(), k.width = 0, k.height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.width = 100, k.height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.width = 500, k.height = 100;
    EXPECT_FALSE(e.check(k));

    e = MediaQueryExpression(kMediaFeatureDeviceAspectRatio, 100, 100);
    k = MediaFeatures(), k.device_width = 0, k.device_height = 100;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.device_width = 100, k.device_height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.device_width = 500, k.device_height = 100;
    EXPECT_FALSE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMinDeviceAspectRatio, 100, 100);
    k = MediaFeatures(), k.device_width = 0, k.device_height = 100;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.device_width = 100, k.device_height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.device_width = 500, k.device_height = 100;
    EXPECT_TRUE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMaxDeviceAspectRatio, 100, 100);
    k = MediaFeatures(), k.device_width = 0, k.device_height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.device_width = 100, k.device_height = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.device_width = 500, k.device_height = 100;
    EXPECT_FALSE(e.check(k));

    e = MediaQueryExpression(kMediaFeatureColor, 100);
    k = MediaFeatures(), k.color = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.color = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.color = 500;
    EXPECT_FALSE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMinColor, 100);
    k = MediaFeatures(), k.color = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.color = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.color = 500;
    EXPECT_TRUE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMaxColor, 100);
    k = MediaFeatures(), k.color = 0;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.color = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.color = 500;
    EXPECT_FALSE(e.check(k));

    e = MediaQueryExpression(kMediaFeatureColorIndex, 100);
    k = MediaFeatures(), k.color_index = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.color_index = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.color_index = 500;
    EXPECT_FALSE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMinColorIndex, 100);
    k = MediaFeatures(), k.color_index = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.color_index = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.color_index = 500;
    EXPECT_TRUE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMaxColorIndex, 100);
    k = MediaFeatures(), k.color_index = 0;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.color_index = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.color_index = 500;
    EXPECT_FALSE(e.check(k));

    e = MediaQueryExpression(kMediaFeatureMonochrome, 100);
    k = MediaFeatures(), k.monochrome = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.monochrome = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.monochrome = 500;
    EXPECT_FALSE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMinMonochrome, 100);
    k = MediaFeatures(), k.monochrome = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.monochrome = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.monochrome = 500;
    EXPECT_TRUE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMaxMonochrome, 100);
    k = MediaFeatures(), k.monochrome = 0;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.monochrome = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.monochrome = 500;
    EXPECT_FALSE(e.check(k));

    e = MediaQueryExpression(kMediaFeatureResolution, 100);
    k = MediaFeatures(), k.resolution = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.resolution = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.resolution = 500;
    EXPECT_FALSE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMinResolution, 100);
    k = MediaFeatures(), k.resolution = 0;
    EXPECT_FALSE(e.check(k));
    k = MediaFeatures(), k.resolution = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.resolution = 500;
    EXPECT_TRUE(e.check(k));
    e = MediaQueryExpression(kMediaFeatureMaxResolution, 100);
    k = MediaFeatures(), k.resolution = 0;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.resolution = 100;
    EXPECT_TRUE(e.check(k));
    k = MediaFeatures(), k.resolution = 500;
    EXPECT_FALSE(e.check(k));
}
