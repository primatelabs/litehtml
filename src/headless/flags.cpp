// Copyright (C) 2020-2025 Primate Labs Inc. All rights reserved.
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
//    * Neither the name of the copyright holder nor the names of its
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

#include "flags.h"

#include <fmt/format.h>

#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>

namespace {

enum Switches {
    kSwitchFirst = 256,

    kSwitchHelp,

    kSwitchFontDirectory,

    kSwitchFile,
    kSwitchURL,

    kSwitchWidth,
    kSwitchHeight,
    kSwitchOutput,
};

constexpr int kDefaultWidth = 768;
constexpr int kDefaultHeight = 1366;

} // namespace

Flags::Flags()
: font_directory(std::filesystem::current_path())
, width(kDefaultWidth)
, height(kDefaultHeight)
, output("headless.png")
{
}

ParseStatus Flags::parse(int argc, char** argv)
{
    argv0 = argv[0];

    int c;
    struct option longopts[] = {
        {"help", no_argument, nullptr, kSwitchHelp},

        {"font-directory", required_argument, nullptr, kSwitchFontDirectory},

        {"file", required_argument, nullptr, kSwitchFile},
        {"url", required_argument, nullptr, kSwitchURL},

        {"width", required_argument, nullptr, kSwitchWidth},
        {"height", required_argument, nullptr, kSwitchHeight},
        {"output", required_argument, nullptr, kSwitchOutput},
        {nullptr, 0, nullptr}
    };

    while ((c = getopt_long(argc, argv, "h?", longopts, nullptr)) != -1) {
        const char* tmp_optarg = optarg;
        switch (c) {
            case kSwitchHelp:
            case 'h':
            case '?':
                do_usage = true;
                break;

            case kSwitchFontDirectory:
                font_directory = optarg;
                break;

            case kSwitchFile:
                file = optarg;
                break;

            case kSwitchURL:
                url = optarg;
                break;

            case kSwitchWidth:
                width = atoi(optarg);
                break;

            case kSwitchHeight:
                height = atoi(optarg);
                break;

            case kSwitchOutput:
                output = optarg;
                break;

            default:
                break;
        }
    }

    return kParseSuccess;
}

void Flags::usage(int exit_code)
{
    std::cout << fmt::format("{} [ options ]\n\n", argv0);
    std::cout << "Options:\n";
    std::cout << "  -h, --help                  print this message\n";
    std::cout << "  ---font-directory DIR       load fonts from DIR directory\n";
    std::cout << "  ---file FILE                load and render the web page from FILE\n";
    std::cout << "  ---url URL                  load and render the web page from URL\n";
    std::cout << "  ---width WIDTH              set the viewport to WIDTH pixels wide\n";
    std::cout << "  ---height HEIGHT            set the viewport to HEIGHT pixels wide\n";
    std::cout << "  ---output PNG               save the rendered web page to PNG\n";
    std::cout << std::endl;

    exit(exit_code);
}
