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

#ifndef LITEHTML_CSS_RULE_H__
#define LITEHTML_CSS_RULE_H__

#include <memory>
#include <vector>

#include "litehtml/css/css_block.h"
#include "litehtml/css/css_prelude.h"
#include "litehtml/debug/json.h"
#include "litehtml/os_types.h"

namespace litehtml {

    class CSSComponentValue;

class CSSRule {
protected:
    String name_;

    std::unique_ptr<CSSPrelude> prelude_;

    std::unique_ptr<CSSBlock> block_;

public:
    CSSRule();

    ~CSSRule() = default;

    const String& name() const
    {
        return name_;
    }

    void name(const String& name)
    {
        name_ = name;
    }

    CSSPrelude* prelude()
    {
        return prelude_.get();
    }

    const CSSPrelude* prelude() const
    {
        return prelude_.get();
    }

    void prelude(CSSPrelude* prelude)
    {
        prelude_.reset(prelude);
    }

    CSSBlock* block()
    {
        return block_.get();
    }

    const CSSBlock* block() const
    {
        return block_.get();
    }

    void block(CSSBlock* block)
    {
        block_.reset(block);
    }

#if defined(ENABLE_JSON)
    nlohmann::json json() const;
#endif // ENABLE_JSON
};

} // namespace litehtml

#endif // LITEHTML_CSS_RULE_H__
