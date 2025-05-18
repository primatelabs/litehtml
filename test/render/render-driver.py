#!/usr/bin/env python3

# Copyright (C) 2020-2025 Primate Labs Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#    * Neither the name of the copyright holder nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import argparse
import glob
import os
import sys
import tempfile
import pathlib
import subprocess

from PIL import Image
from pixelmatch.contrib.PIL import pixelmatch


def run_headless(headless_executable, html_path, output_path):
    html_files = glob.glob(os.path.join(html_path, '*.html'))

    for html_file in html_files:
        html_filename = pathlib.Path(html_file).name
        png_filename = pathlib.Path(html_filename).with_suffix('.png')

        cmd = [
            headless_executable,
            '--file', html_file,
            '--output', os.path.join(output_path, png_filename)
        ]
        subprocess.run(cmd, check=True)


def compare_images(output_path, reference_path):
    status = 0
    png_files = glob.glob(os.path.join(output_path, '*.png'))

    for png_file in png_files:
        png_filename = pathlib.Path(png_file).name

        output = Image.open(os.path.join(output_path, png_filename))
        reference = Image.open(os.path.join(reference_path, png_filename))
        delta = Image.new("RGBA", reference.size)

        mismatch = pixelmatch(output, reference, delta, includeAA=True)
        print(png_filename, mismatch)

        if mismatch > 0:
            status = 1

    return status

def argument_parser():
    parser = argparse.ArgumentParser()

    parser.add_argument('action')

    parser.add_argument('--headless-executable', required=True)

    parser.add_argument('--html-path', required=True)

    parser.add_argument('--reference-path', required=True)

    return parser


def main():
    parser = argument_parser()
    args = parser.parse_args()

    status = 0

    if args.action == 'test':
        tmp_path = '/tmp'
        run_headless(args.headless_executable,
            args.html_path,
            tmp_path)

        status |= compare_images(tmp_path, args.reference_path)

    if args.action == 'regenerate':
        run_headless(args.headless_executable,
            args.html_path,
            args.reference_path)

    # sys.exit() only handles values between 0 and 127. Python documentation
    # states that behaviour for values outside of this range may be undefined,
    # but in our experience values are masked against 0x7f. Values that mask to
    # 0 are therefore treated as success. Work around this problem by comparing
    # the value against 0 and returning 0 for success, 1 for failure.
    sys.exit(status != 0)

if __name__ == '__main__':
    main()