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

#include "litehtml/document.h"
#include "litehtml/document_container.h"
#include "litehtml/html.h"

namespace litehtml {

bool MediaQueryExpression::check(const MediaFeatures& features) const
{
    switch (feature) {
        case kMediaFeatureWidth:
            if (check_as_bool) {
                return (features.width != 0);
            } else if (features.width == val) {
                return true;
            }
            break;
        case kMediaFeatureMinWidth:
            if (features.width >= val) {
                return true;
            }
            break;
        case kMediaFeatureMaxWidth:
            if (features.width <= val) {
                return true;
            }
            break;
        case kMediaFeatureHeight:
            if (check_as_bool) {
                return (features.height != 0);
            } else if (features.height == val) {
                return true;
            }
            break;
        case kMediaFeatureMinHeight:
            if (features.height >= val) {
                return true;
            }
            break;
        case kMediaFeatureMaxHeight:
            if (features.height <= val) {
                return true;
            }
            break;

        case kMediaFeatureDeviceWidth:
            if (check_as_bool) {
                return (features.device_width != 0);
            } else if (features.device_width == val) {
                return true;
            }
            break;
        case kMediaFeatureMinDeviceWidth:
            if (features.device_width >= val) {
                return true;
            }
            break;
        case kMediaFeatureMaxDeviceWidth:
            if (features.device_width <= val) {
                return true;
            }
            break;
        case kMediaFeatureDeviceHeight:
            if (check_as_bool) {
                return (features.device_height != 0);
            } else if (features.device_height == val) {
                return true;
            }
            break;
        case kMediaFeatureMinDeviceHeight:
            if (features.device_height >= val) {
                return true;
            }
            break;
        case kMediaFeatureMaxDeviceHeight:
            if (features.device_height <= val) {
                return true;
            }
            break;

        case kMediaFeatureOrientation:
            if (features.height >= features.width) {
                if (val == kMediaOrientationPortrait) {
                    return true;
                }
            } else {
                if (val == kMediaOrientationLandscape) {
                    return true;
                }
            }
            break;
        case kMediaFeatureAspectRatio:
            if (features.height && val2) {
                int ratio_this = round_d((double)val / (double)val2 * 100);
                int ratio_feat = round_d(
                    (double)features.width / (double)features.height * 100.0);
                if (ratio_this == ratio_feat) {
                    return true;
                }
            }
            break;
        case kMediaFeatureMinAspectRatio:
            if (features.height && val2) {
                int ratio_this = round_d((double)val / (double)val2 * 100);
                int ratio_feat = round_d(
                    (double)features.width / (double)features.height * 100.0);
                if (ratio_feat >= ratio_this) {
                    return true;
                }
            }
            break;
        case kMediaFeatureMaxAspectRatio:
            if (features.height && val2) {
                int ratio_this = round_d((double)val / (double)val2 * 100);
                int ratio_feat = round_d(
                    (double)features.width / (double)features.height * 100.0);
                if (ratio_feat <= ratio_this) {
                    return true;
                }
            }
            break;

        case kMediaFeatureDeviceAspectRatio:
            if (features.device_height && val2) {
                int ratio_this = round_d((double)val / (double)val2 * 100);
                int ratio_feat = round_d((double)features.device_width /
                                         (double)features.device_height * 100.0);
                if (ratio_feat == ratio_this) {
                    return true;
                }
            }
            break;
        case kMediaFeatureMinDeviceAspectRatio:
            if (features.device_height && val2) {
                int ratio_this = round_d((double)val / (double)val2 * 100);
                int ratio_feat = round_d((double)features.device_width /
                                         (double)features.device_height * 100.0);
                if (ratio_feat >= ratio_this) {
                    return true;
                }
            }
            break;
        case kMediaFeatureMaxDeviceAspectRatio:
            if (features.device_height && val2) {
                int ratio_this = round_d((double)val / (double)val2 * 100);
                int ratio_feat = round_d((double)features.device_width /
                                         (double)features.device_height * 100.0);
                if (ratio_feat <= ratio_this) {
                    return true;
                }
            }
            break;

        case kMediaFeatureColor:
            if (check_as_bool) {
                return (features.color != 0);
            } else if (features.color == val) {
                return true;
            }
            break;
        case kMediaFeatureMinColor:
            if (features.color >= val) {
                return true;
            }
            break;
        case kMediaFeatureMaxColor:
            if (features.color <= val) {
                return true;
            }
            break;

        case kMediaFeatureColorIndex:
            if (check_as_bool) {
                return (features.color_index != 0);
            } else if (features.color_index == val) {
                return true;
            }
            break;
        case kMediaFeatureMinColorIndex:
            if (features.color_index >= val) {
                return true;
            }
            break;
        case kMediaFeatureMaxColorIndex:
            if (features.color_index <= val) {
                return true;
            }
            break;

        case kMediaFeatureMonochrome:
            if (check_as_bool) {
                return (features.monochrome != 0);
            } else if (features.monochrome == val) {
                return true;
            }
            break;
        case kMediaFeatureMinMonochrome:
            if (features.monochrome >= val) {
                return true;
            }
            break;
        case kMediaFeatureMaxMonochrome:
            if (features.monochrome <= val) {
                return true;
            }
            break;

        case kMediaFeatureResolution:
            if (features.resolution == val) {
                return true;
            }
            break;
        case kMediaFeatureMinResolution:
            if (features.resolution >= val) {
                return true;
            }
            break;
        case kMediaFeatureMaxResolution:
            if (features.resolution <= val) {
                return true;
            }
            break;
        default:
            return false;
    }

    return false;
}

} // namespace litehtml
