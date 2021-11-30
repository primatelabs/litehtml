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

#include "litehtml/element/text_element.h"

#include <iostream>

#include "litehtml/document.h"
#include "litehtml/document_container.h"
#include "litehtml/html.h"

namespace litehtml {

TextElement::TextElement(const tchar_t* text, Document* doc)
: Element(doc)
{
    if (text) {
        text_ = text;
    }
    text_transform_ = kTextTransformNone;
    use_transformed_ = false;
    draw_spaces_ = true;
}

TextElement::~TextElement()
{
}

void TextElement::get_content_size(Size& sz, int /* max_width */)
{
    sz = size_;
}

void TextElement::get_text(tstring& text)
{
    text += text_;
}

const tchar_t* TextElement::get_style_property(CSSProperty name)
{
    // TextElement is an internal element created by litehtml (i.e., text
    // elements do not appear in the HTML itself).  Since it does not appear
    // in the HTML it cannot have its own CSS styles.  Instead, all of its CSS
    // styles come directly from its parent.

    // TODO: Should we even check whether properties are inherited?  Or should
    // we just grab every property from the parent node regardless of whether
    // or not it can be inherited?

    bool inherited = css_property_inherited(name);

    if (inherited) {
        if (parent()) {
            return parent()->get_style_property(name);
        }
    }
    return css_property_default(name);
}

void TextElement::parse_styles(bool /* is_reparse */)
{
    text_transform_ = (TextTransform)value_index(
        get_style_property(kCSSPropertyTextTransform),
        TEXT_TRANSFORM_STRINGS,
        kTextTransformNone);

    if (text_transform_ != kTextTransformNone) {
        transformed_text_ = text_;
        use_transformed_ = true;
        get_document()->container()->transform_text(transformed_text_,
            text_transform_);
    }

    if (is_white_space()) {
        transformed_text_ = _t(" ");
        use_transformed_ = true;
    } else {
        if (text_ == _t("\t")) {
            transformed_text_ = _t("    ");
            use_transformed_ = true;
        }
        if (text_ == _t("\n") || text_ == _t("\r")) {
            transformed_text_ = _t("");
            use_transformed_ = true;
        }
    }

    FontMetrics fm;
    uintptr_t font = 0;
    Element::ptr el_parent = parent();
    if (el_parent) {
        font = el_parent->get_font(&fm);
    }
    if (is_break()) {
        size_.height = 0;
        size_.width = 0;
    } else {
        size_.height = fm.height;
        size_.width = get_document()->container()->text_width(
            use_transformed_ ? transformed_text_.c_str() : text_.c_str(),
            font);
    }
    draw_spaces_ = fm.draw_spaces;
}

int TextElement::get_baseline()
{
    Element::ptr el_parent = parent();
    if (el_parent) {
        return el_parent->get_baseline();
    }
    return 0;
}

void TextElement::draw(uintptr_t hdc, int x, int y, const Position* clip)
{
    if (is_white_space() && !draw_spaces_) {
        return;
    }

    Position pos = position_;
    pos.x += x;
    pos.y += y;

    if (pos.does_intersect(clip)) {
        Element::ptr el_parent = parent();
        if (el_parent) {
            Document* doc = get_document();

            uintptr_t font = el_parent->get_font();
            WebColor color =
                el_parent->get_color(kCSSPropertyColor, doc->get_default_color());
            doc->container()->draw_text(hdc,
                use_transformed_ ? transformed_text_.c_str() : text_.c_str(),
                font,
                color,
                pos);
        }
    }
}

int TextElement::line_height() const
{
    Element::ptr el_parent = parent();
    if (el_parent) {
        return el_parent->line_height();
    }
    return 0;
}

uintptr_t TextElement::get_font(FontMetrics* fm /*= 0*/)
{
    Element::ptr el_parent = parent();
    if (el_parent) {
        return el_parent->get_font(fm);
    }
    return 0;
}

Display TextElement::get_display() const
{
    return kDisplayInlineText;
}

WhiteSpace TextElement::get_white_space() const
{
    Element::ptr el_parent = parent();
    if (el_parent)
        return el_parent->get_white_space();
    return kWhiteSpaceNormal;
}

ElementPosition TextElement::get_element_position(CSSOffsets* offsets) const
{
    Element::ptr p = parent();
    while (p && p->get_display() == kDisplayInline) {
        if (p->get_element_position() == kPositionRelative) {
            if (offsets) {
                *offsets = p->get_css_offsets();
            }
            return kPositionRelative;
        }
        p = p->parent();
    }
    return kPositionStatic;
}

CSSOffsets TextElement::get_css_offsets() const
{
    Element::ptr p = parent();
    while (p && p->get_display() == kDisplayInline) {
        if (p->get_element_position() == kPositionRelative) {
            return p->get_css_offsets();
        }
        p = p->parent();
    }
    return CSSOffsets();
}

} // namespace litehtml
