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

#include "litehtml/document.h"

#include <math.h>
#include <stdio.h>

#include <algorithm>

#include <gumbo.h>
#include <utf8cpp/utf8.h>


#include "litehtml/css/css_stylesheet.h"
#include "litehtml/document.h"
#include "litehtml/document_container.h"
#include "litehtml/element/anchor_element.h"
#include "litehtml/element/base_element.h"
#include "litehtml/element/body_element.h"
#include "litehtml/element/break_element.h"
#include "litehtml/element/cdata_element.h"
#include "litehtml/element/comment_element.h"
#include "litehtml/element/div_element.h"
#include "litehtml/element/font_element.h"
#include "litehtml/element/image_element.h"
#include "litehtml/element/li_element.h"
#include "litehtml/element/link_element.h"
#include "litehtml/element/paragraph_element.h"
#include "litehtml/element/script_element.h"
#include "litehtml/element/whitespace_element.h"
#include "litehtml/element/style_element.h"
#include "litehtml/element/table_element.h"
#include "litehtml/element/td_element.h"
#include "litehtml/element/text_element.h"
#include "litehtml/element/title_element.h"
#include "litehtml/element/tr_element.h"
#include "litehtml/html.h"
#include "litehtml/element/html_element.h"
#include "litehtml/utf8_strings.h"
#include "litehtml/logging.h"
#include "litehtml/text.h"
#include "litehtml/string_view.h"

#if defined(USE_ICU)

#include "unicode/brkiter.h"
#include "unicode/udata.h"

using namespace icu;

#endif

namespace litehtml {
namespace {

// Split a Gumbo text node into one or more litehtml text elements.  Each text
// element contains a single indivisible string of text (e.g., a word).  This
// approach simplifies the renderer as the parser computes and caches the text
// extents while the renderer only has to draw each individual element.

#if defined(USE_ICU)

void split_text_node(Document* document, ElementsVector& elements, const char* text)
{
    UErrorCode code = U_ZERO_ERROR;
    BreakIterator* break_iterator =
        BreakIterator::createLineInstance(Locale::getEnglish(), code);

    break_iterator->setText(text);

    int32_t start = break_iterator->first();
    for (int32_t end = break_iterator->next(); end != BreakIterator::DONE;
         start = end, end = break_iterator->next()) {
        std::string str(text + start, end - start);
        elements.push_back(new TextElement(document, str.c_str()));
    }
}

#else

void split_text_node(Document* document, ElementsVector& elements, const char* text)
{
    const char* end = text + strlen(text);

    StringView str;
    StringView wss;

    utf8::iterator<const char*> iterator(text, text, text + strlen(text));

    const char* p = text;
    while (p != end) {
        const char* prev = p;
        char32_t c = utf8::next(p, end);

        if (is_whitespace(c)) {
            if (!str.empty()) {
                elements.push_back(new TextElement(document, str.data(), str.length()));
                str = StringView();
            }
            if (wss.empty()) {
                wss = StringView(prev, (p - prev));
            } else {
                wss = StringView(wss.data(), (p - wss.data()));
            }
        } else {
            if (!wss.empty()) {
                elements.push_back(new WhitespaceElement(document, wss.data(), wss.length()));
                wss = StringView();
            }
            if (str.empty()) {
                str = StringView(prev, (p - prev));
            } else {
                str = StringView(str.data(), (p - str.data()));
            }
        }
    }

    if (!str.empty()) {
        elements.push_back(new TextElement(document, str.data(), str.length()));
        str = StringView();
    }

    if (!wss.empty()) {
        elements.push_back(new WhitespaceElement(document, wss.data(), wss.length()));
        wss = StringView();
    }
}

#endif

} // namespace

Document::Document(DocumentContainer* container, Context* context)
: container_(container)
, context_(context)
{
}

Document::Document(const URL& base_url,
    DocumentContainer* container,
    Context* context)
: container_(container)
, context_(context)
, base_url_(base_url)
{
}

Document::~Document()
{
    m_over_element = nullptr;
    if (container_) {
        for (FontMap::iterator f = m_fonts.begin(); f != m_fonts.end(); f++) {
            container_->delete_font(f->second.font);
        }
    }
}

uintptr_t Document::add_font(const char* name,
    int size,
    const char* weight,
    const char* style,
    const char* decoration,
    FontMetrics* fm)
{
    uintptr_t ret = 0;

    if (!name || (name && !t_strcasecmp(name, "inherit"))) {
        name = container_->get_default_font_name();
    }

    if (!size) {
        size = container()->get_default_font_size();
    }

    char strSize[20];
    t_itoa(size, strSize, 20, 10);

    std::string key = name;
    key += ":";
    key += strSize;
    key += ":";
    key += weight;
    key += ":";
    key += style;
    key += ":";
    key += decoration;

    if (m_fonts.find(key) == m_fonts.end()) {
        font_style fs =
            (font_style)value_index(style, font_style_strings, fontStyleNormal);
        int fw = value_index(weight, FONT_WEIGHT_STRINGS, -1);
        if (fw >= 0) {
            switch (fw) {
                case kFontWeightBold:
                    fw = 700;
                    break;
                case kFontWeightBolder:
                    fw = 600;
                    break;
                case kFontWeightLighter:
                    fw = 300;
                    break;
                default:
                    fw = 400;
                    break;
            }
        } else {
            fw = atoi(weight);
            if (fw < 100) {
                fw = 400;
            }
        }

        unsigned int decor = 0;

        if (decoration) {
            std::vector<std::string> tokens;
            split_string(decoration, tokens, " ");
            for (std::vector<std::string>::iterator i = tokens.begin();
                 i != tokens.end();
                 i++) {
                if (!t_strcasecmp(i->c_str(), "underline")) {
                    decor |= font_decoration_underline;
                } else if (!t_strcasecmp(i->c_str(), "line-through")) {
                    decor |= font_decoration_linethrough;
                } else if (!t_strcasecmp(i->c_str(), "overline")) {
                    decor |= font_decoration_overline;
                }
            }
        }

        FontItem fi = {0};

        fi.font = container_->create_font(name, size, fw, fs, decor, &fi.metrics);
        m_fonts[key] = fi;
        ret = fi.font;
        if (fm) {
            *fm = fi.metrics;
        }
    }
    return ret;
}

uintptr_t Document::get_font(const char* name,
    int size,
    const char* weight,
    const char* style,
    const char* decoration,
    FontMetrics* fm)
{
    if (!name || (name && !t_strcasecmp(name, "inherit"))) {
        name = container_->get_default_font_name();
    }

    if (!size) {
        size = container_->get_default_font_size();
    }

    char strSize[20];
    t_itoa(size, strSize, 20, 10);

    std::string key = name;
    key += ":";
    key += strSize;
    key += ":";
    key += weight;
    key += ":";
    key += style;
    key += ":";
    key += decoration;

    FontMap::iterator el = m_fonts.find(key);

    if (el != m_fonts.end()) {
        if (fm) {
            *fm = el->second.metrics;
        }
        return el->second.font;
    }
    return add_font(name, size, weight, style, decoration, fm);
}

int Document::render(int max_width)
{
    int ret = 0;
    if (root_) {
        ret = root_->render(0, 0, max_width);
        if (root_->fetch_positioned()) {
            root_->render_positioned();
        }
        m_size.width = 0;
        m_size.height = 0;
        root_->calc_document_size(m_size);
    }
    return ret;
}

void Document::draw(uintptr_t hdc, int x, int y, const Position* clip)
{
    if (root_) {
        root_->draw(hdc, x, y, clip);
        root_->draw_stacking_context(hdc, x, y, clip, true);
    }
}

int Document::cvt_units(const char* str,
    int fontSize,
    bool* is_percent /*= 0*/) const
{
    if (!str)
        return 0;

    CSSLength val;
    val.parse_length_string(str);
    if (is_percent && val.units() == kCSSUnitsPercent && !val.is_predefined()) {
        *is_percent = true;
    }
    return cvt_units(val, fontSize);
}

int Document::cvt_units(CSSLength& val, int fontSize, int size) const
{
    if (val.is_predefined()) {
        return 0;
    }
    int ret = 0;
    switch (val.units()) {
        case kCSSUnitsPercent:
            ret = val.calc_percent(size);
            break;
        case kCSSUnitsEm:
            ret = round_f(val.val() * fontSize);
            val.set_value((float)ret, kCSSUnitsPx);
            break;
        case kCSSUnitsPt:
            ret = container_->pt_to_px((int)val.val());
            val.set_value((float)ret, kCSSUnitsPx);
            break;
        case kCSSUnitsIn:
            ret = container_->pt_to_px((int)(val.val() * 72));
            val.set_value((float)ret, kCSSUnitsPx);
            break;
        case kCSSUnitsCm:
            ret = container_->pt_to_px((int)(val.val() * 0.3937 * 72));
            val.set_value((float)ret, kCSSUnitsPx);
            break;
        case kCSSUnitsMm:
            ret = container_->pt_to_px((int)(val.val() * 0.3937 * 72) / 10);
            val.set_value((float)ret, kCSSUnitsPx);
            break;
        case kCSSUnitsVw:
            ret = (int)((double)m_media.width * (double)val.val() / 100.0);
            break;
        case kCSSUnitsVh:
            ret = (int)((double)m_media.height * (double)val.val() / 100.0);
            break;
        case kCSSUnitsVmin:
            ret = (int)((double)std::min(m_media.height, m_media.width) *
                        (double)val.val() / 100.0);
            break;
        case kCSSUnitsVmax:
            ret = (int)((double)std::max(m_media.height, m_media.width) *
                        (double)val.val() / 100.0);
            break;
        case kCSSUnitsRem:
            ret = (int)((double)root_->get_font_size() * (double)val.val());
            val.set_value((float)ret, kCSSUnitsPx);
            break;
        default:
            ret = (int)val.val();
            break;
    }
    return ret;
}

int Document::width() const
{
    return m_size.width;
}

int Document::height() const
{
    return m_size.height;
}

void Document::add_javascript(const std::string& str, const URL& url)
{
    if (!str.empty()) {
        m_scripts.push_back(str);
    }
}

void Document::add_stylesheet(const std::string& str,
    const URL& url,
    const char* media)
{
    if (!str.empty()) {
        m_css.push_back(css_text(str, url, media));
    }
}

bool Document::on_mouse_over(int x,
    int y,
    int client_x,
    int client_y,
    std::vector<Position>& redraw_boxes)
{
    if (!root_) {
        return false;
    }

    Element::ptr over_el = root_->get_element_by_point(x, y, client_x, client_y);

    bool state_was_changed = false;

    if (over_el != m_over_element) {
        if (m_over_element) {
            if (m_over_element->on_mouse_leave()) {
                state_was_changed = true;
            }
        }
        m_over_element = over_el;
    }

    const char* cursor = nullptr;

    if (m_over_element) {
        if (m_over_element->on_mouse_over()) {
            state_was_changed = true;
        }
        cursor = m_over_element->get_cursor();
    }

    container_->set_cursor(cursor ? cursor : "auto");

    if (state_was_changed) {
        return root_->find_styles_changes(redraw_boxes, 0, 0);
    }
    return false;
}

bool Document::on_mouse_leave(std::vector<Position>& redraw_boxes)
{
    if (!root_) {
        return false;
    }
    if (m_over_element) {
        if (m_over_element->on_mouse_leave()) {
            return root_->find_styles_changes(redraw_boxes, 0, 0);
        }
    }
    return false;
}

bool Document::on_lbutton_down(int x,
    int y,
    int client_x,
    int client_y,
    std::vector<Position>& redraw_boxes)
{
    if (!root_) {
        return false;
    }

    Element::ptr over_el = root_->get_element_by_point(x, y, client_x, client_y);

    bool state_was_changed = false;

    if (over_el != m_over_element) {
        if (m_over_element) {
            if (m_over_element->on_mouse_leave()) {
                state_was_changed = true;
            }
        }
        m_over_element = over_el;
        if (m_over_element) {
            if (m_over_element->on_mouse_over()) {
                state_was_changed = true;
            }
        }
    }

    const char* cursor = nullptr;

    if (m_over_element) {
        if (m_over_element->on_lbutton_down()) {
            state_was_changed = true;
        }
        cursor = m_over_element->get_cursor();
    }

    container_->set_cursor(cursor ? cursor : "auto");

    if (state_was_changed) {
        return root_->find_styles_changes(redraw_boxes, 0, 0);
    }

    return false;
}

bool Document::on_lbutton_up(int,
    int,
    int,
    int,
    std::vector<Position>& redraw_boxes)
{
    if (!root_) {
        return false;
    }
    if (m_over_element) {
        if (m_over_element->on_lbutton_up()) {
            return root_->find_styles_changes(redraw_boxes, 0, 0);
        }
    }
    return false;
}

Element::ptr Document::create_element(const char* tag_name,
    const string_map& attributes)
{
    Element* newTag = nullptr;

    if (!strcmp(tag_name, "br")) {
        newTag = new BreakElement(this);
    } else if (!strcmp(tag_name, "p")) {
        newTag = new ParagraphElement(this);
    } else if (!strcmp(tag_name, "img")) {
        newTag = new ImageElement(this);
    } else if (!strcmp(tag_name, "table")) {
        newTag = new TableElement(this);
    } else if (!strcmp(tag_name, "td") || !strcmp(tag_name, "th")) {
        newTag = new TdElement(this);
    } else if (!strcmp(tag_name, "link")) {
        newTag = new LinkElement(this);
    } else if (!strcmp(tag_name, "title")) {
        newTag = new TitleElement(this);
    } else if (!strcmp(tag_name, "a")) {
        newTag = new AnchorElement(this);
    } else if (!strcmp(tag_name, "tr")) {
        newTag = new TrElement(this);
    } else if (!strcmp(tag_name, "style")) {
        newTag = new StyleElement(this);
    } else if (!strcmp(tag_name, "base")) {
        newTag = new BaseElement(this);
    } else if (!strcmp(tag_name, "body")) {
        newTag = new BodyElement(this);
    } else if (!strcmp(tag_name, "div")) {
        newTag = new DivElement(this);
    } else if (!strcmp(tag_name, "script")) {
        newTag = new ScriptElement(this);
    } else if (!strcmp(tag_name, "font")) {
        newTag = new FontElement(this);
    } else if (!strcmp(tag_name, "li")) {
        newTag = new LiElement(this);
    } else {
        newTag = new HTMLElement(this);
    }

    if (newTag) {
        newTag->set_tagName(tag_name);
        for (string_map::const_iterator iter = attributes.begin();
             iter != attributes.end();
             iter++) {
            newTag->set_attr(iter->first.c_str(), iter->second.c_str());
        }
    }

    return newTag;
}

bool Document::media_changed()
{
    if (!m_media_lists.empty()) {
        container()->get_media_features(m_media);
        if (update_media_lists(m_media)) {
            root_->refresh_styles();
            root_->parse_styles();
            return true;
        }
    }
    return false;
}

bool Document::lang_changed()
{
    if (!m_media_lists.empty()) {
        std::string culture;
        container()->get_language(language_, culture);
        if (!culture.empty()) {
            culture_ = language_ + '-' + culture;
        } else {
            culture_.clear();
        }
        root_->refresh_styles();
        root_->parse_styles();
        return true;
    }
    return false;
}

bool Document::update_media_lists(const MediaFeatures& features)
{
    bool update_styles = false;
    for (MediaQueryList::vector::iterator iter = m_media_lists.begin();
         iter != m_media_lists.end();
         iter++) {
        if ((*iter)->apply_MediaFeatures(features)) {
            update_styles = true;
        }
    }
    return update_styles;
}

void Document::add_media_list(MediaQueryList::ptr list)
{
    if (list) {
        if (std::find(m_media_lists.begin(), m_media_lists.end(), list) ==
            m_media_lists.end()) {
            m_media_lists.push_back(list);
        }
    }
}

void Document::create_node(void* gnode, ElementsVector& elements, bool parseTextNode)
{
    GumboNode* node = (GumboNode*)gnode;
    switch (node->type) {
        case GUMBO_NODE_ELEMENT: {
            string_map attrs;
            GumboAttribute* attr;
            for (unsigned int i = 0; i < node->v.element.attributes.length; i++) {
                attr = (GumboAttribute*)node->v.element.attributes.data[i];
                attrs[String(attr->name)] = attr->value;
            }


            Element* ret = nullptr;
            const char* tag = gumbo_normalized_tagname(node->v.element.tag);
            if (tag[0]) {
                ret = create_element(tag, attrs);
            } else {
                if (node->v.element.original_tag.data &&
                    node->v.element.original_tag.length) {
                    std::string strA;
                    gumbo_tag_from_original_text(&node->v.element.original_tag);
                    strA.append(node->v.element.original_tag.data,
                        node->v.element.original_tag.length);
                    ret = create_element(strA.c_str(), attrs);
                }
            }
            if (!strcmp(tag, "script")) {
                parseTextNode = false;
            }
            if (ret) {
                ElementsVector child;
                for (unsigned int i = 0; i < node->v.element.children.length; i++) {
                    child.clear();
                    create_node(node->v.element.children.data[i],
                        child,
                        parseTextNode);
                    ret->append_children(child);
                }
                elements.push_back(ret);
            }
        } break;
        case GUMBO_NODE_TEXT: {
            const char* text = node->v.text.text;

            if (!parseTextNode) {
                elements.push_back(new TextElement(this, text));
                break;
            } else {
                split_text_node(this, elements, text);
            }
        } break;
        case GUMBO_NODE_CDATA: {
            Element::ptr ret = new CDATAElement(this);
            ret->set_data(node->v.text.text);
            elements.push_back(ret);
        } break;
        case GUMBO_NODE_COMMENT: {
            Element::ptr ret = new CommentElement(this);
            ret->set_data(node->v.text.text);
            elements.push_back(ret);
        } break;
        case GUMBO_NODE_WHITESPACE: {
            const char* text = node->v.text.text;
            elements.push_back(new WhitespaceElement(this, text));
        } break;
        default:
            break;
    }
}

void Document::fix_tables_layout()
{
    size_t i = 0;
    while (i < m_tabular_elements.size()) {
        Element::ptr el_ptr = m_tabular_elements[i];

        switch (el_ptr->get_display()) {
            case kDisplayInlineTable:
            case kDisplayTable:
                fix_table_children(el_ptr,
                    kDisplayTableRowGroup,
                    "table-row-group");
                break;
            case kDisplayTableFooterGroup:
            case kDisplayTableRowGroup:
            case kDisplayTableHeaderGroup: {
                Element::ptr parent = el_ptr->parent();
                if (parent) {
                    if (parent->get_display() != kDisplayInlineTable)
                        fix_table_parent(el_ptr, kDisplayTable, "table");
                }
                fix_table_children(el_ptr, kDisplayTableRow, "table-row");
            } break;
            case kDisplayTableRow:
                fix_table_parent(el_ptr,
                    kDisplayTableRowGroup,
                    "table-row-group");
                fix_table_children(el_ptr, kDisplayTableCell, "table-cell");
                break;
            case kDisplayTableCell:
                fix_table_parent(el_ptr, kDisplayTableRow, "table-row");
                break;
            // TODO: make table layout fix for table-caption, table-column etc. elements
            case kDisplayTableCaption:
            case kDisplayTableColumn:
            case kDisplayTableColumnGroup:
            default:
                break;
        }
        i++;
    }
}

void Document::fix_table_children(Element::ptr& el_ptr,
    Display disp,
    const char* disp_str)
{
    ElementsVector tmp;
    ElementsVector::iterator first_iter = el_ptr->m_children.begin();
    ElementsVector::iterator cur_iter = el_ptr->m_children.begin();

    auto flush_elements = [&]() {
        Element::ptr annon_tag = new HTMLElement(this);
        CSSStyle st;
        st.add_property(kCSSPropertyDisplay, disp_str, URL(), false);
        annon_tag->add_style(st);
        annon_tag->parent(el_ptr);
        annon_tag->parse_styles();
        annon_tag->append_children(tmp);
        first_iter = el_ptr->m_children.insert(first_iter, annon_tag);
        cur_iter = first_iter + 1;
        while (cur_iter != el_ptr->m_children.end() &&
               (*cur_iter)->parent() != el_ptr) {
            cur_iter = el_ptr->m_children.erase(cur_iter);
        }
        first_iter = cur_iter;
        tmp.clear();
    };

    while (cur_iter != el_ptr->m_children.end()) {
        if ((*cur_iter)->get_display() != disp) {
            if (!(*cur_iter)->is_whitespace() ||
                ((*cur_iter)->is_whitespace() && !tmp.empty())) {
                if (tmp.empty()) {
                    first_iter = cur_iter;
                }
                tmp.push_back((*cur_iter));
            }
            cur_iter++;
        } else if (!tmp.empty()) {
            flush_elements();
        } else {
            cur_iter++;
        }
    }
    if (!tmp.empty()) {
        flush_elements();
    }
}

void Document::fix_table_parent(Element::ptr& el_ptr,
    Display disp,
    const char* disp_str)
{
    Element::ptr parent = el_ptr->parent();

    if (parent->get_display() != disp) {
        ElementsVector::iterator this_element =
            std::find_if(parent->m_children.begin(),
                parent->m_children.end(),
                [&](Element::ptr& el) {
                    if (el == el_ptr) {
                        return true;
                    }
                    return false;
                });
        if (this_element != parent->m_children.end()) {
            Display el_disp = el_ptr->get_display();
            ElementsVector::iterator first = this_element;
            ElementsVector::iterator last = this_element;
            ElementsVector::iterator cur = this_element;

            // find first element with same display
            while (true) {
                if (cur == parent->m_children.begin())
                    break;
                cur--;
                if ((*cur)->is_whitespace() || (*cur)->get_display() == el_disp) {
                    first = cur;
                } else {
                    break;
                }
            }

            // find last element with same display
            cur = this_element;
            while (true) {
                cur++;
                if (cur == parent->m_children.end())
                    break;

                if ((*cur)->is_whitespace() || (*cur)->get_display() == el_disp) {
                    last = cur;
                } else {
                    break;
                }
            }

            // extract elements with the same display and wrap them with anonymous object
            Element* annon_tag = new HTMLElement(this);
            CSSStyle st;
            st.add_property(kCSSPropertyDisplay, disp_str, URL(), false);
            annon_tag->add_style(st);
            annon_tag->parent(parent);
            annon_tag->parse_styles();
            std::for_each(first, last + 1, [&annon_tag](Element* element) {
                if (!annon_tag->append_child(element)) {
                    delete element;
                }
            });
            first = parent->m_children.erase(first, last + 1);
            parent->m_children.insert(first, annon_tag);
        }
    }
}

void Document::append_children_from_string(Element& parent, const char* str)
{
    append_children_from_utf8(parent, str);
}

void Document::append_children_from_utf8(Element& parent, const char* str)
{
    // parent must belong to this document
    if (parent.get_document() != this) {
        return;
    }

    // parse document into GumboOutput
    GumboOutput* output = gumbo_parse((const char*)str);

    // Create elements.
    ElementsVector child_elements;
    create_node(output->root, child_elements, true);

    // Destroy GumboOutput
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    // Let's process created elements tree
    for (Element::ptr child : child_elements) {
        // Add the child element to parent
        parent.append_child(child);

        // apply master CSS
        child->apply_stylesheet(context_->master_stylesheet());

        // parse elements attributes
        child->parse_attributes();

        // Apply parsed styles.
        child->apply_stylesheet(stylesheet_);

        // Parse applied styles in the elements
        child->parse_styles();

        // Now the m_tabular_elements is filled with tabular elements.
        // We have to check the tabular elements for missing table elements
        // and create the anonymous boxes in visual table layout
        fix_tables_layout();

        // Fanaly initialize elements
        child->init();
    }
}

#if defined(ENABLE_JSON)

nlohmann::json Document::json() const
{
    return nlohmann::json{
        {"root", root_->json()}
    };
}

#endif // ENABLE_JSON

} // namespace litehtml
