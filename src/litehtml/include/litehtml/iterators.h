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

#ifndef LITEHTML_ITERATORS_H__
#define LITEHTML_ITERATORS_H__

#include "litehtml/element/element.h"
#include "litehtml/types.h"

namespace litehtml {
class iterator_selector {
public:
    virtual bool select(const element::ptr& el) = 0;
};

class elements_iterator {
private:
    struct stack_item {
        int idx;
        element::ptr el;
        stack_item()
        {
        }
        stack_item(const stack_item& val)
        {
            idx = val.idx;
            el = val.el;
        }
        stack_item(stack_item&& val)
        {
            idx = val.idx;
            el = std::move(val.el);
        }
    };

    std::vector<stack_item> m_stack;
    element::ptr m_el;
    int m_idx;
    iterator_selector* m_go_inside;
    iterator_selector* m_select;

public:
    elements_iterator(const element::ptr& el,
        iterator_selector* go_inside,
        iterator_selector* select)
    {
        m_el = el;
        m_idx = -1;
        m_go_inside = go_inside;
        m_select = select;
    }

    ~elements_iterator()
    {
    }

    element::ptr next(bool ret_parent = true);

private:
    void next_idx();
};

class go_inside_inline : public iterator_selector {
public:
    virtual bool select(const element::ptr& el);
};

class go_inside_table : public iterator_selector {
public:
    virtual bool select(const element::ptr& el);
};

class table_rows_selector : public iterator_selector {
public:
    virtual bool select(const element::ptr& el);
};

class table_cells_selector : public iterator_selector {
public:
    virtual bool select(const element::ptr& el);
};
} // namespace litehtml

#endif // LITEHTML_ITERATORS_H__
