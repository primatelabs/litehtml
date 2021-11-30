#!/usr/bin/env python

# Copyright (c) 2020-2021 Primate Labs Inc.
# All rights reserved.
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
#    * Neither the names of the copyright holders nor the names of their
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

from __future__ import print_function

import argparse
import chevron
import json
import os
import sys

def css_property_type(str):
    words = str.split('-')
    for i in range(0, len(words)):
        words[i] = words[i].capitalize()
    return 'kCSSProperty{}'.format(''.join(words))

def css_property_default(css_property):
    default = css_property.get('default', None)
    if default is None:
        return 'nullptr'
    else:
        return '"{}"'.format(default)

def css_property_inherited(css_property):
    inherited = css_property.get('inherited', True)
    if inherited is True:
        return 'true'
    else:
        return 'false'

def css_property_value_type(css_property):
    value_type = css_property.get('value-type', 'string')
    return 'kCSSValue{}'.format(value_type.title())

def enumeration_case(str):
    return str.replace('-', ' ').title().replace(' ', '')

def format_keyword_enum(prefix, keyword):
    return 'k{}{}'.format(prefix, keyword)

def css_property_keywords(name, css_property):
    valid_keywords = css_property.get('valid-keywords', [])
    prefix = css_property.get('keyword-prefix', enumeration_case(name))

    result = []

    for keyword in valid_keywords:
        result.append({
            'keyword_str': keyword,
            'keyword_enum': format_keyword_enum(prefix, enumeration_case(keyword))
        })

    return result

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("json")
    parser.add_argument("cpp_output")
    parser.add_argument("h_output")
    args = parser.parse_args()

    css_properties = json.load(open(args.json))

    properties = []
    keywords = []
    for name in css_properties:
        css_property = css_properties[name]

        properties.append({
            'property_name': name,
            'property_enum': css_property_type(name),
            'property_default': css_property_default(css_property),
            'property_inherited': css_property_inherited(css_property),
            'property_value_type': css_property_value_type(css_property),
            'keywords': css_property_keywords(name, css_property)

        })

        valid_keywords = css_property.get('valid-keywords', [])
        for keyword in valid_keywords:
            keywords.append({
                'keyword': format_keyword_enum(name, keyword)
            })

    d = {
        'properties': properties,
        'keywords': keywords
    }

    cpp_template_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'css_property.cpp.mustache')
    cpp_template_str = open(cpp_template_path).read()
    open(args.cpp_output, 'w').write(chevron.render(cpp_template_str, d))

    h_template_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'css_property.h.mustache')
    h_template_str = open(h_template_path).read()
    open(args.h_output, 'w').write(chevron.render(h_template_str, d))



if __name__ == '__main__':
    main()
