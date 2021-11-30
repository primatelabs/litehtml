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

#ifndef LITEHTML_LOGGING_H__
#define LITEHTML_LOGGING_H__

#include <iomanip>
#include <sstream>

#include "litehtml/string.h"

namespace litehtml {

// Sets the log level. Anything at or above this level will be written to the
// log file/displayed to the user (if applicable). Anything below this level
// will be silently ignored. The log level defaults to 0 (everything is logged)
// if this function is not called.
void SetMinLogLevel(int level);

// Gets the current log level.
int GetMinLogLevel();

typedef int LogSeverity;
const LogSeverity LOG_INFO = 0;
const LogSeverity LOG_WARNING = 1;
const LogSeverity LOG_ERROR = 2;
const LogSeverity LOG_ERROR_REPORT = 3;
const LogSeverity LOG_FATAL = 4;
const LogSeverity LOG_NUM_SEVERITIES = 5;

// A few definitions of macros that don't generate much code. These are used
// by LOG() and LOG_IF, etc. Since these are used all over our code, it's
// better to have compact code for these operations.
#define COMPACT_LITEHTML_LOG_EX_INFO(ClassName, ...) \
    litehtml::ClassName(__FILE__, __LINE__, litehtml::LOG_INFO, ##__VA_ARGS__)
#define COMPACT_LITEHTML_LOG_EX_WARNING(ClassName, ...) \
    litehtml::ClassName(__FILE__, __LINE__, litehtml::LOG_WARNING, ##__VA_ARGS__)
#define COMPACT_LITEHTML_LOG_EX_ERROR(ClassName, ...) \
    litehtml::ClassName(__FILE__, __LINE__, litehtml::LOG_ERROR, ##__VA_ARGS__)
#define COMPACT_LITEHTML_LOG_EX_ERROR_REPORT(ClassName, ...) \
    litehtml::ClassName(__FILE__, __LINE__, litehtml::LOG_ERROR_REPORT, ##__VA_ARGS__)
#define COMPACT_LITEHTML_LOG_EX_FATAL(ClassName, ...) \
    litehtml::ClassName(__FILE__, __LINE__, litehtml::LOG_FATAL, ##__VA_ARGS__)
#define COMPACT_LITEHTML_LOG_EX_DFATAL(ClassName, ...) \
    litehtml::ClassName(__FILE__, __LINE__, litehtml::LOG_DFATAL_LEVEL, ##__VA_ARGS__)

#define COMPACT_LITEHTML_LOG_INFO COMPACT_LITEHTML_LOG_EX_INFO(LogMessage)
#define COMPACT_LITEHTML_LOG_WARNING COMPACT_LITEHTML_LOG_EX_WARNING(LogMessage)
#define COMPACT_LITEHTML_LOG_ERROR COMPACT_LITEHTML_LOG_EX_ERROR(LogMessage)
#define COMPACT_LITEHTML_LOG_ERROR_REPORT \
    COMPACT_LITEHTML_LOG_EX_ERROR_REPORT(LogMessage)
#define COMPACT_LITEHTML_LOG_FATAL COMPACT_LITEHTML_LOG_EX_FATAL(LogMessage)
#define COMPACT_LITEHTML_LOG_DFATAL COMPACT_LITEHTML_LOG_EX_DFATAL(LogMessage)

// wingdi.h defines ERROR to be 0. When we call LOG(ERROR), it gets
// substituted with 0, and it expands to COMPACT_LITEHTML_LOG_0. To allow us
// to keep using this syntax, we define this macro to do the same thing
// as COMPACT_LITEHTML_LOG_ERROR, and also define ERROR the same way that
// the Windows SDK does for consistency.
#define ERROR 0
#define COMPACT_LITEHTML_LOG_EX_0(ClassName, ...) \
    COMPACT_LITEHTML_LOG_EX_ERROR(ClassName, ##__VA_ARGS__)
#define COMPACT_LITEHTML_LOG_0 COMPACT_LITEHTML_LOG_ERROR

// We use the preprocessor's merging operator, "##", so that, e.g.,
// LOG(INFO) becomes the token COMPACT_LITEHTML_LOG_INFO.  There's some funny
// subtle difference between ostream member streaming functions (e.g.,
// ostream::operator<<(int) and ostream non-member streaming functions
// (e.g., ::operator<<(ostream&, string&): it turns out that it's
// impossible to stream something like a string directly to an unnamed
// ostream. We employ a neat hack by calling the stream() member
// function of LogMessage which seems to avoid the problem.

#define LOG(severity) COMPACT_LITEHTML_LOG_##severity.stream()

// This class more or less represents a particular log message.  You
// create an instance of LogMessage and then stream stuff to it.
// When you finish streaming to it, ~LogMessage is called and the
// full message gets streamed to the appropriate destination.
//
// You shouldn't actually use LogMessage's constructor to log things,
// though.  You should use the LOG() macro (and variants thereof)
// above.
class LogMessage {
public:
    LogMessage(const char* file, int line, LogSeverity severity, int ctr);

    // Two special constructors that generate reduced amounts of code at
    // LOG call sites for common cases.
    //
    // Used for LOG(INFO): Implied are:
    // severity = LOG_INFO, ctr = 0
    //
    // Using this constructor instead of the more complex constructor above
    // saves a couple of bytes per call site.
    LogMessage(const char* file, int line);

    // Used for LOG(severity) where severity != INFO.  Implied
    // are: ctr = 0
    //
    // Using this constructor instead of the more complex constructor above
    // saves a couple of bytes per call site.
    LogMessage(const char* file, int line, LogSeverity severity);

    ~LogMessage();

    std::ostream& stream()
    {
        return stream_;
    }

private:
    void Init(const char* file, int line);

    LogSeverity severity_;
    std::ostringstream stream_;
    size_t message_start_; // Offset of the start of the message (past prefix
                           // info).
    // DISALLOW_COPY_AND_ASSIGN(LogMessage);
};

} // namespace litehtml

#endif // LITEHTML_LOGGING_H__
