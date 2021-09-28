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

#ifndef LITEHTML_OS_TYPES_H__
#define LITEHTML_OS_TYPES_H__

#include <string.h>

#include <string>

namespace litehtml {
#if defined(WIN32) || defined(_WIN32) || defined(WINCE)

#ifndef LITEHTML_UTF8

typedef std::wstring tstring;
typedef wchar_t tchar_t;
typedef std::wstringstream tstringstream;

#define _t(quote) L##quote

#define t_strlen wcslen
#define t_strcmp wcscmp
#define t_strncmp wcsncmp
#define t_strcasecmp _wcsicmp
#define t_strncasecmp _wcsnicmp
#define t_strtol wcstol
#define t_atoi _wtoi
#define t_strtod wcstod
#define t_itoa(value, buffer, size, radix) _itow_s(value, buffer, size, radix)
#define t_strstr wcsstr
#define t_tolower towlower
#define t_isdigit iswdigit
#define t_to_string(val) std::to_wstring(val)

#else

typedef std::string tstring;
typedef char tchar_t;
typedef std::stringstream tstringstream;

#define _t(quote) quote

#define t_strlen strlen
#define t_strcmp strcmp
#define t_strncmp strncmp
#define t_strcasecmp _stricmp
#define t_strncasecmp _strnicmp
#define t_strtol strtol
#define t_atoi atoi
#define t_strtod strtod
#define t_itoa(value, buffer, size, radix) _itoa_s(value, buffer, size, radix)
#define t_strstr strstr
#define t_tolower tolower
#define t_isdigit isdigit
#define t_to_string(val) std::to_string(val)

#endif

#ifdef _WIN64
typedef unsigned __int64 uint_ptr;
#else
typedef unsigned int uint_ptr;
#endif

#else
#define LITEHTML_UTF8

typedef std::string tstring;
typedef char tchar_t;
typedef std::uintptr_t uint_ptr;
typedef std::stringstream tstringstream;

#define _t(quote) quote

#define t_strlen strlen
#define t_strcmp strcmp
#define t_strncmp strncmp

#define t_strcasecmp strcasecmp
#define t_strncasecmp strncasecmp
#define t_itoa(value, buffer, size, radix) snprintf(buffer, size, "%d", value)

#define t_strtol strtol
#define t_atoi atoi
#define t_strtod strtod
#define t_strstr strstr
#define t_tolower tolower
#define t_isdigit isdigit
#define t_to_string(val) std::to_string(val)

#endif
} // namespace litehtml

#endif // LITEHTML_OS_TYPES_H__