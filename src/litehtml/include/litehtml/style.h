// Copyright (c) 2013, Yuri Kobets (tordex)
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

#ifndef LITEHTML_STYLE_H__
#define LITEHTML_STYLE_H__
#include <string>

#include "attributes.h"

namespace litehtml {

class property_value {
public:
    tstring m_value;

    bool m_important;

    property_value()
    {
        m_important = false;
    }
    property_value(const tchar_t* val, bool imp)
    {
        m_important = imp;
        m_value = val;
    }
    property_value(const property_value& val)
    {
        m_value = val.m_value;
        m_important = val.m_important;
    }

    property_value& operator=(const property_value& val)
    {
        m_value = val.m_value;
        m_important = val.m_important;
        return *this;
    }
};

typedef std::map<tstring, property_value> props_map;

class style {
public:
    typedef std::shared_ptr<style> ptr;
    typedef std::vector<style::ptr> vector;

private:
    props_map m_properties;
    static string_map m_valid_values;

public:
    style();
    style(const style& val);
    virtual ~style();

    void operator=(const style& val)
    {
        m_properties = val.m_properties;
    }

    void add(const tchar_t* txt, const tchar_t* baseurl)
    {
        parse(txt, baseurl);
    }

    void add_property(const tchar_t* name,
        const tchar_t* val,
        const tchar_t* baseurl,
        bool important);

    const tchar_t* get_property(const tchar_t* name) const
    {
        if (name) {
            props_map::const_iterator f = m_properties.find(name);
            if (f != m_properties.end()) {
                return f->second.m_value.c_str();
            }
        }
        return nullptr;
    }

    void combine(const litehtml::style& src);

    void clear()
    {
        m_properties.clear();
    }

private:
    void parse_property(const tstring& txt, const tchar_t* baseurl);
    void parse(const tchar_t* txt, const tchar_t* baseurl);
    void parse_short_border(const tstring& prefix, const tstring& val, bool important);
    void parse_short_background(const tstring& val,
        const tchar_t* baseurl,
        bool important);
    void parse_short_font(const tstring& val, bool important);
    void add_parsed_property(const tstring& name, const tstring& val, bool important);
    void remove_property(const tstring& name, bool important);
};
} // namespace litehtml

#endif // LITEHTML_STYLE_H__