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

#ifndef LITEHTML_STRING_H__
#define LITEHTML_STRING_H__

#include <string.h>

#include <string>

namespace litehtml {

typedef std::string tstring;
typedef char tchar_t;
typedef std::stringstream tstringstream;

#define _t(quote) quote

#define t_strlen strlen
#define t_strcmp strcmp
#define t_strncmp strncmp
#define t_strtol strtol
#define t_atoi atoi
#define t_strtod strtod
#define t_strstr strstr
#define t_tolower tolower
#define t_isdigit isdigit
#define t_to_string(val) std::to_string(val)

#if defined(WIN32) || defined(_WIN32)

#define t_strcasecmp _stricmp
#define t_strncasecmp _strnicmp
#define t_itoa(value, buffer, size, radix) _itoa_s(value, buffer, size, radix)

#else

#define t_strcasecmp strcasecmp
#define t_strncasecmp strncasecmp
#define t_itoa(value, buffer, size, radix) snprintf(buffer, size, "%d", value)

#endif

using String = tstring;

} // namespace litehtml

#endif // LITEHTML_STRING_H__
