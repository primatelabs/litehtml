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

#ifndef LITEHTML_MEDIA_QUERY_H__
#define LITEHTML_MEDIA_QUERY_H__
#include <vector>

#include "litehtml/types.h"

namespace litehtml {
struct media_query_expression {
    typedef std::vector<media_query_expression> vector;
    media_feature feature;
    int val;
    int val2;
    bool check_as_bool;

    media_query_expression()
    {
        check_as_bool = false;
        feature = media_feature_none;
        val = 0;
        val2 = 0;
    }

    bool check(const media_features& features) const;
};

class media_query {
public:
    typedef std::shared_ptr<media_query> ptr;
    typedef std::vector<media_query::ptr> vector;

private:
    media_query_expression::vector m_expressions;
    bool m_not;
    media_type m_media_type;

public:
    media_query();
    media_query(const media_query& val);

    static media_query::ptr create_from_string(const tstring& str,
        const std::shared_ptr<document>& doc);
    bool check(const media_features& features) const;
};

class media_query_list {
public:
    typedef std::shared_ptr<media_query_list> ptr;
    typedef std::vector<media_query_list::ptr> vector;

private:
    media_query::vector m_queries;
    bool m_is_used;

public:
    media_query_list();
    media_query_list(const media_query_list& val);

    static media_query_list::ptr create_from_string(const tstring& str,
        const std::shared_ptr<document>& doc);
    bool is_used() const;
    bool apply_media_features(
        const media_features& features); // returns true if the m_is_used changed
};

inline media_query_list::media_query_list(const media_query_list& val)
{
    m_is_used = val.m_is_used;
    m_queries = val.m_queries;
}

inline media_query_list::media_query_list()
{
    m_is_used = false;
}

inline bool media_query_list::is_used() const
{
    return m_is_used;
}

} // namespace litehtml

#endif // LITEHTML_MEDIA_QUERY_H__