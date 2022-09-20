// Copyright (c) 2013, Yuri Kobets (tordex)
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
//    * Neither the name of the copyright holders nor the names of their
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

#ifndef LITEHTML_CSS_SELECTOR_H__
#define LITEHTML_CSS_SELECTOR_H__

#include <memory>

#include "litehtml/css/css_style.h"
#include "litehtml/media_query_list.h"

namespace litehtml {

//////////////////////////////////////////////////////////////////////////

struct CSSSelectorSpecificity {
    int a;
    int b;
    int c;
    int d;

    CSSSelectorSpecificity(int va = 0, int vb = 0, int vc = 0, int vd = 0)
    {
        a = va;
        b = vb;
        c = vc;
        d = vd;
    }

    void operator+=(const CSSSelectorSpecificity& val)
    {
        a += val.a;
        b += val.b;
        c += val.c;
        d += val.d;
    }

    bool operator==(const CSSSelectorSpecificity& val) const
    {
        if (a == val.a && b == val.b && c == val.c && d == val.d) {
            return true;
        }
        return false;
    }

    bool operator!=(const CSSSelectorSpecificity& val) const
    {
        if (a != val.a || b != val.b || c != val.c || d != val.d) {
            return true;
        }
        return false;
    }

    bool operator>(const CSSSelectorSpecificity& val) const
    {
        if (a > val.a) {
            return true;
        } else if (a < val.a) {
            return false;
        } else {
            if (b > val.b) {
                return true;
            } else if (b < val.b) {
                return false;
            } else {
                if (c > val.c) {
                    return true;
                } else if (c < val.c) {
                    return false;
                } else {
                    if (d > val.d) {
                        return true;
                    } else if (d < val.d) {
                        return false;
                    }
                }
            }
        }
        return false;
    }

    bool operator>=(const CSSSelectorSpecificity& val) const
    {
        if ((*this) == val)
            return true;
        if ((*this) > val)
            return true;
        return false;
    }

    bool operator<=(const CSSSelectorSpecificity& val) const
    {
        if ((*this) > val) {
            return false;
        }
        return true;
    }

    bool operator<(const CSSSelectorSpecificity& val) const
    {
        if ((*this) <= val && (*this) != val) {
            return true;
        }
        return false;
    }

#if defined(ENABLE_JSON)
    nlohmann::json json() const
    {
        return nlohmann::json{
            {"a", a},
            {"b", b},
            {"c", c},
            {"d", d},
        };
    }
#endif
};

//////////////////////////////////////////////////////////////////////////

enum CSSAttributeSelectCondition {
    kSelectExists,
    kSelectEqual,
    kSelectContainStr,
    kSelectStartStr,
    kSelectEndStr,
    kSelectPseudoClass,
    kSelectPseudoElement,
};

//////////////////////////////////////////////////////////////////////////

struct CSSAttributeSelector {
    typedef std::vector<CSSAttributeSelector> vector;

    tstring attribute;
    tstring val;
    string_vector class_val;
    CSSAttributeSelectCondition condition;

    CSSAttributeSelector()
    {
        condition = kSelectExists;
    }

#if defined(ENABLE_JSON)
    nlohmann::json json() const
    {
        return nlohmann::json{
            {"attribute", attribute},
            {"value", val},
            {"class_val", json_vector(class_val)},
        };
    }
#endif

};

//////////////////////////////////////////////////////////////////////////

class CSSElementSelector {
public:
    String m_tag;
    CSSAttributeSelector::vector m_attrs;

public:
    void parse(const String& txt);

#if defined(ENABLE_JSON)
    nlohmann::json json() const
    {
        return nlohmann::json{
            {"tag", m_tag},
            {"attrs", json_vector(m_attrs)},
        };
    }
#endif

};

//////////////////////////////////////////////////////////////////////////

enum CSSCombinator {
    kCombinatorDescendant,
    kCombinatorChild,
    kCombinatorAdjacentSibling,
    kCombinatorGeneralSibling
};

//////////////////////////////////////////////////////////////////////////

class CSSSelector {
public:
    typedef std::shared_ptr<CSSSelector> ptr;
    typedef std::vector<CSSSelector::ptr> vector;

public:
    CSSSelectorSpecificity m_specificity;
    CSSElementSelector m_right;
    CSSSelector::ptr m_left;
    CSSCombinator m_combinator;
    CSSStyle::ptr m_style;
    size_t m_order;
    MediaQueryList::ptr media_query_list_;

public:
    CSSSelector(MediaQueryList::ptr media)
    {
        media_query_list_ = media;
        m_combinator = kCombinatorDescendant;
        m_order = 0;
    }

    ~CSSSelector()
    {
    }

    CSSSelector(const CSSSelector& val)
    {
        m_right = val.m_right;
        if (val.m_left) {
            m_left = std::make_shared<CSSSelector>(*val.m_left);
        } else {
            m_left = nullptr;
        }
        m_combinator = val.m_combinator;
        m_specificity = val.m_specificity;
        m_order = val.m_order;
        media_query_list_ = val.media_query_list_;
    }

    bool parse(const tstring& text);
    void calc_specificity();
    bool is_media_valid() const;
    void add_media_to_doc(Document* doc) const;

#if defined(ENABLE_JSON)
    nlohmann::json json() const;
#endif
};

inline bool CSSSelector::is_media_valid() const
{
    if (!media_query_list_) {
        return true;
    }
    return media_query_list_->is_used();
}


//////////////////////////////////////////////////////////////////////////

inline bool operator>(const CSSSelector& v1, const CSSSelector& v2)
{
    if (v1.m_specificity == v2.m_specificity) {
        return (v1.m_order > v2.m_order);
    }
    return (v1.m_specificity > v2.m_specificity);
}

inline bool operator<(const CSSSelector& v1, const CSSSelector& v2)
{
    if (v1.m_specificity == v2.m_specificity) {
        return (v1.m_order < v2.m_order);
    }
    return (v1.m_specificity < v2.m_specificity);
}

inline bool operator>(const CSSSelector::ptr& v1, const CSSSelector::ptr& v2)
{
    return (*v1 > *v2);
}

inline bool operator<(const CSSSelector::ptr& v1, const CSSSelector::ptr& v2)
{
    return (*v1 < *v2);
}

//////////////////////////////////////////////////////////////////////////

class used_selector {
public:
    typedef std::unique_ptr<used_selector> ptr;
    typedef std::vector<used_selector::ptr> vector;

    CSSSelector::ptr m_selector;
    bool m_used;

    used_selector(const CSSSelector::ptr& selector, bool used)
    {
        m_used = used;
        m_selector = selector;
    }
};

} // namespace litehtml

#endif // LITEHTML_CSS_SELECTOR_H__
