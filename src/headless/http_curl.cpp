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

#include "http_curl.h"

#include <curl/curl.h>
#include <curl/easy.h>
#include <openssl/crypto.h>
#include <openssl/md5.h>
#include <stdint.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "http.h"

namespace {

uint32_t write_callback(char* in, uint32_t size, uint32_t nmemb, std::string* out)
{
    uint32_t r = size * nmemb;
    out->append(in, r);
    return r;
}

} // namespace

http_response http_request(const litehtml::URL& url)
{
    static bool init_curl = false;

    if (!init_curl) {
        curl_global_init(CURL_GLOBAL_ALL);
        init_curl = true;
    }

    http_response response;

    CURL* curl = curl_easy_init();
    if (!curl) {
        response.code = 0;
        return response;
    }

    // TODO: Find a way to bundle CA certs so that verification can occur.
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    curl_easy_setopt(curl, CURLOPT_URL, url.string().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.body);

    CURLcode result = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &response.code);

    if (result != CURLE_OK && result != CURLE_PARTIAL_FILE) {
        response.code = 0;
    }

    return response;
}
