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

#ifndef LITEHTML_DOCUMENT_H__
#define LITEHTML_DOCUMENT_H__

#include <memory>
#include <vector>

#include "litehtml/color.h"
#include "litehtml/context.h"
#include "litehtml/css/css_style.h"
#include "litehtml/debug/json.h"
#include "litehtml/element/element.h"
#include "litehtml/types.h"
#include "litehtml/url.h"

namespace litehtml {

struct css_text {
    typedef std::vector<css_text> vector;

    std::string text;
    URL baseurl;
    std::string media;

    css_text()
    {
    }

    css_text(const std::string& txt, const URL& url, const char* media_str)
    : text(txt)
    , baseurl(url)
    {
        media = media_str ? media_str : "";
    }

    css_text(const css_text& val)
    {
        text = val.text;
        baseurl = val.baseurl;
        media = val.media;
    }
};

class HTMLElement;

class Document : public std::enable_shared_from_this<Document> {
public:
    typedef std::shared_ptr<Document> ptr;
    typedef std::weak_ptr<Document> weak_ptr;

private:
    std::unique_ptr<Element> root_;

    DocumentContainer* container_;

    FontMap m_fonts;

    css_text::vector m_css;

    std::vector<std::string> m_scripts;

    CSSStylesheet stylesheet_;

    Color default_color_;

    Context* context_;

    litehtml::Size m_size;

    MediaQueryList::vector m_media_lists;

    Element::ptr m_over_element;

    ElementsVector m_tabular_elements;

    MediaFeatures m_media;

    std::string language_;

    std::string culture_;

    URL base_url_;

public:
    Document(litehtml::DocumentContainer* objContainer, Context* ctx);

    Document(const URL& base_url,
        litehtml::DocumentContainer* objContainer,
        Context* ctx);

    virtual ~Document();

    Element* root()
    {
        return root_.get();
    }

    DocumentContainer* container() const
    {
        return container_;
    }

    const CSSStylesheet& stylesheet() const
    {
        return stylesheet_;
    }

    uintptr_t get_font(const char* name,
        int size,
        const char* weight,
        const char* style,
        const char* decoration,
        FontMetrics* fm);

    int render(int max_width);

    void draw(uintptr_t hdc, int x, int y, const Position* clip);

    Color get_default_color()
    {
        return default_color_;
    }

    int cvt_units(const char* str, int fontSize, bool* is_percent = nullptr) const;

    int cvt_units(CSSLength& val, int fontSize, int size = 0) const;

    int width() const;

    int height() const;

    void add_javascript(const std::string& str,
        const URL& url);

    void add_stylesheet(const std::string& str,
        const URL& url,
        const char* media);

    bool on_mouse_over(int x,
        int y,
        int client_x,
        int client_y,
        std::vector<Position>& redraw_boxes);
    bool on_lbutton_down(int x,
        int y,
        int client_x,
        int client_y,
        std::vector<Position>& redraw_boxes);
    bool on_lbutton_up(int x,
        int y,
        int client_x,
        int client_y,
        std::vector<Position>& redraw_boxes);
    bool on_mouse_leave(std::vector<Position>& redraw_boxes);
    litehtml::Element::ptr create_element(const char* tag_name,
        const string_map& attributes);

    void add_media_list(MediaQueryList::ptr list);
    bool media_changed();
    bool lang_changed();
    bool match_lang(const std::string& lang)
    {
        return lang == language_ || lang == culture_;
    }

    void add_tabular(const Element::ptr& el)
    {
        m_tabular_elements.push_back(el);
    }

    const Element::const_ptr get_over_element() const
    {
        return m_over_element;
    }

    const URL& base_url() const
    {
        return base_url_;
    }

    void base_url(const URL& base_url)
    {
        base_url_ = base_url;
    }

    void append_children_from_string(Element& parent, const char* str);

    void append_children_from_utf8(Element& parent, const char* str);

private:
    uintptr_t add_font(const char* name,
        int size,
        const char* weight,
        const char* style,
        const char* decoration,
        FontMetrics* fm);

    void create_node(void* gnode, ElementsVector& elements, bool parseTextNode);
    bool update_media_lists(const MediaFeatures& features);
    void fix_tables_layout();
    void fix_table_children(Element::ptr& el_ptr,
        Display disp,
        const char* disp_str);
    void fix_table_parent(Element::ptr& el_ptr,
        Display disp,
        const char* disp_str);

public:
#if defined(ENABLE_JSON)
    nlohmann::json json() const;
#endif // ENABLE_JSON

    friend class DocumentParser;
};

} // namespace litehtml

#endif // LITEHTML_DOCUMENT_H__
