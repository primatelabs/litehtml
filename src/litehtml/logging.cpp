// Copyright (c) 2006-2008 The Chromium Authors.
// Copyright (C) 2020-2021 Primate Labs Inc.
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

#include "litehtml/logging.h"

#include <vector>

namespace litehtml {

namespace {

std::vector<String> log_severity_names = {"INFO",
    "WARNING",
    "ERROR",
    "ERROR_REPORT",
    "FATAL"};

int min_log_level = 0;

// what should be prepended to each message?
bool log_timestamp = true;

} // namespace

void SetMinLogLevel(int level)
{
    min_log_level = level;
}

// Gets the current log level.
int GetMinLogLevel()
{
    return min_log_level;
}

LogMessage::LogMessage(const char* file, int line, LogSeverity severity, int)
: severity_(severity)
{
    Init(file, line);
}

LogMessage::LogMessage(const char* file, int line)
: severity_(LOG_INFO)
{
    Init(file, line);
}

LogMessage::LogMessage(const char* file, int line, LogSeverity severity)
: severity_(severity)
{
    Init(file, line);
}

// writes the common header info to the stream
void LogMessage::Init(const char* file, int line)
{
    // log only the filename

    const char* last_slash = strrchr(file, '/');
    if (last_slash) {
        file = last_slash + 1;
    }

    // TODO(darin): It might be nice if the columns were fixed width.

    stream_ << '[';
    if (log_timestamp) {
        time_t t = time(nullptr);
        struct tm local_time = {0};
#if _MSC_VER >= 1400
        localtime_s(&local_time, &t);
#else
        localtime_r(&t, &local_time);
#endif
        struct tm* tm_time = &local_time;
        stream_ << std::setfill('0') << std::setw(2) << 1 + tm_time->tm_mon
                << std::setw(2) << tm_time->tm_mday << '/' << std::setw(2)
                << tm_time->tm_hour << std::setw(2) << tm_time->tm_min
                << std::setw(2) << tm_time->tm_sec << ':';
    }
    stream_ << log_severity_names[severity_] << ":" << file << "(" << line << ")] ";

    message_start_ = stream_.tellp();
}

LogMessage::~LogMessage()
{
    // TODO(brettw) modify the macros so that nothing is executed when the log
    // level is too high.
    if (severity_ < min_log_level)
        return;

    std::string str_newline(stream_.str());
#if defined(PLATFORM_WINDOWS)
    str_newline.append("\r\n");
#else
    str_newline.append("\n");
#endif

    fprintf(stderr, "%s", str_newline.c_str());
    fflush(stderr);
}

} // namespace litehtml
