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

#ifndef LITEHTML_UTF8_STRINGS_H__
#define LITEHTML_UTF8_STRINGS_H__

#include "litehtml/types.h"

namespace litehtml {
class utf8_to_wchar {
    const byte* m_utf8;
    std::wstring m_str;

public:
    utf8_to_wchar(const char* val);
    operator const wchar_t*() const
    {
        return m_str.c_str();
    }

private:
    ucode_t getb()
    {
        if (!(*m_utf8))
            return 0;
        return *m_utf8++;
    }
    ucode_t get_next_utf8(ucode_t val)
    {
        return (val & 0x3f);
    }
    ucode_t get_char();
};

class wchar_to_utf8 {
    std::string m_str;

public:
    wchar_to_utf8(const std::wstring& val);
    operator const char*() const
    {
        return m_str.c_str();
    }

    const char* c_str() const
    {
        return m_str.c_str();
    }
};

#define litehtml_from_wchar(str) litehtml::wchar_to_utf8(str)

} // namespace litehtml

#endif // LITEHTML_UTF8_STRINGS_H__
