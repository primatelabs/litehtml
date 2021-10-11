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

#include "http_darwin.h"

#import <Foundation/Foundation.h>

#include "http.h"

http_response http_request(const litehtml::URL& url)
{
  // TODO: Figure out how to enable ARC using CMake for Objective-C and
  // Objective-C++ files on macOS.
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

  NSURL* u = [NSURL URLWithString:[NSString stringWithUTF8String:url.string().c_str()]];
  NSURLRequest* req = [NSURLRequest requestWithURL:u];

  // Send the request to the server.

  NSURLResponse* res = nullptr;
  NSError* error = nullptr;
  NSData* data = [NSURLConnection sendSynchronousRequest:req returningResponse:&res error:&error];

  // Parse the response from the server.

  http_response response;

  // Determine the HTTP status code (if available).

  if (res && [res respondsToSelector:@selector(statusCode)]) {
    response.code = [res statusCode];
  }

  // Determine if the request returned any data.  If it did, make sure we
  // didn't receive an HTTP error code (in which case the data is probably the
  // contents or the error page).  If it didn't, report the error message (if
  // available).

  if (data) {
    response.body.append((char*)[data bytes], [data length]);
  }

  [pool release];

  return response;
}

