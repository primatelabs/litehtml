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

#include "headless_container.h"

#include <fstream>

#include <fmt/format.h>
#include <orion/arc.h>
#include <orion/conv_stroke.h>
#include <orion/rounded_rect.h>
#include <utf8cpp/utf8.h>

#include "http.h"
#include "image/jpeg_codec.h"
#include "image/png_codec.h"
#include "litehtml/logging.h"
#include "orion_render_context.h"

using namespace litehtml;

#if defined(ENABLE_TRACE)

#define HEADLESS_TRACE0(fn) LOG(INFO) << #fn;

#define HEADLESS_TRACE1(fn, p0) LOG(INFO) << fmt::format("{} {}", #fn, (p0));

#define HEADLESS_TRACE2(fn, p0, p1) \
    LOG(INFO) << fmt::format("{} {} {}", #fn, (p0), (p1));

#define HEADLESS_TRACE3(fn, p0, p1, p2) \
    LOG(INFO) << fmt::format("{} {} {} {}", #fn, (p0), (p1), (p2));

#define HEADLESS_TRACE4(fn, p0, p1, p2, p3) \
    LOG(INFO) << fmt::format("{} {} {} {} {}", #fn, (p0), (p1), (p2), (p3));

#define HEADLESS_TRACE5(fn, p0, p1, p2, p3, p4) \
    LOG(INFO) << fmt::format("{} {} {} {} {} {}", #fn, (p0), (p1), (p2), (p3), (p4));

#else

#define HEADLESS_TRACE0(fn)

#define HEADLESS_TRACE1(fn, p0)

#define HEADLESS_TRACE2(fn, p0, p1)

#define HEADLESS_TRACE3(fn, p0, p1, p2)

#define HEADLESS_TRACE4(fn, p0, p1, p2, p3)

#define HEADLESS_TRACE5(fn, p0, p1, p2, p3, p4)

#endif


namespace headless {

namespace {

constexpr int kDefaultWidth = 1800;
constexpr int kDefaultHeight = 900;

// We cannot use a higher DPI setting until media queries are implemented.
constexpr int kDefaultDPI = 72;

void draw_bitmap(Image<uint8_t>& canvas, FT_Bitmap* bitmap, Color color, FT_Int x, FT_Int y)
{
    FT_Int x_max = x + bitmap->width;
    FT_Int y_max = y + bitmap->rows;

    // For simplicity, assume that `bitmap->pixel_mode` is `FT_PIXEL_MODE_GRAY`
    // (i.e., not a bitmap font).

    for (FT_Int i = x, p = 0; i < x_max; i++, p++) {
        for (FT_Int j = y, q = 0; j < y_max; j++, q++) {
            if (i < 0 || j < 0 || i >= canvas.width() || j >= canvas.height()) {
                continue;
            }

            uint8_t* pixel = canvas.pixel(i, j);
            uint8_t level = bitmap->buffer[q * bitmap->width + p];

            // TODO: Optimize with SIMD?
            // TODO: Figure out what this actually does -- are we blending here?
            pixel[0] = ((pixel[0] * (255 - level)) + (level * color.red)) / 255;
            pixel[1] = ((pixel[1] * (255 - level)) + (level * color.green)) / 255;
            pixel[2] = ((pixel[2] * (255 - level)) + (level * color.blue)) / 255;
            pixel[3] = pixel[3];
        }
    }
}

class Path {
protected:
  std::vector<int> points_;
  int offset_;

public:
  Path(const std::vector<int>& points)
  : points_(points)
  {
  }

  void rewind(int path = 0)
  {
    offset_ = 0;
  }

  unsigned vertex(double* x, double* y)
  {
    if (offset_ < points_.size()) {
      *x = static_cast<double>(points_[offset_++]);
      *y = static_cast<double>(points_[offset_++]);
      return (offset_ == 2 ? orion::path_cmd_move_to : orion::path_cmd_line_to);
    }
    return orion::path_cmd_stop;
  }
};

orion::rgba8 convert_color(const litehtml::Color& color)
{
    return orion::rgba8(color.red, color.green, color.blue, color.alpha);
}

} // namespace

HeadlessContainer::HeadlessContainer()
: DocumentContainer()
, dpi_(kDefaultDPI)
{
    FT_CALL(FT_Init_FreeType(&library_));
}

HeadlessContainer::~HeadlessContainer()
{
}

uintptr_t HeadlessContainer::create_font(const char* faceName,
    int size,
    int weight,
    litehtml::font_style italic,
    unsigned int decoration,
    litehtml::FontMetrics* fm)
{
    HEADLESS_TRACE5(HeadlessContainer::create_font,
        faceName,
        size,
        weight,
        italic,
        decoration);

    // TODO: Is this the correct way to allocate FT_Face structures?
    FT_Face face = nullptr;

    FT_CALL(FT_New_Face(library_,
        "Roboto-Regular.ttf",
        0,
        &face));

    FT_CALL(FT_Set_Char_Size(face, size * 64, 0, dpi_, 0));

    if (fm) {
        // Freetype descender is usually negative, while litehtml expects it
        // to be positive.
        fm->ascent = face->size->metrics.ascender / 64;
        fm->descent = abs(face->size->metrics.descender / 64);

        // The height fields should be equal to the sum of the ascent and
        // descent fields.  During testing it is but we've only tested on a
        // handful of fonts.  Does litehtml break anywhere if this
        // relationship doesn't hold?  Other containers compute height using
        // ascent and descent.
        fm->height = face->size->metrics.height / 64;

        // Calculate x_height using the width of the lower-case x character in
        // the current font.  It's not clear how litehtml uses this variable
        // but other containers compute this value in the same way.
        fm->x_height = text_width("x", (uintptr_t)face);
    }

    return (uintptr_t)(face);
}

void HeadlessContainer::delete_font(uintptr_t hFont)
{
    HEADLESS_TRACE1(HeadlessContainer::delete_font, hFont);

    FT_Face face = (FT_Face)(hFont);
    FT_CALL(FT_Done_Face(face));
}

int HeadlessContainer::text_width(const char* text,
    uintptr_t hFont)
{
    // Don't bother tracing this function as it's called once per token (where
    // a token is a discrete unit of text like a word).  See also draw_text().
    //
    // That said, it can be useful to enable this function to see how litehtml
    // is tokenizing the text on the page.
    //
    // HEADLESS_TRACE1(HeadlessContainer::text_width, text);

    FT_Face face = (FT_Face)(hFont);
    FT_GlyphSlot glyph = face->glyph;
    int width = 0;

    const char* end = text + strlen(text);
    const char* p = text;

    while (p != end) {
        char32_t c = utf8::next(p, end);
        FT_CALL(FT_Load_Char(face, c, FT_LOAD_DEFAULT));
        width += glyph->advance.x;
    }

    // Convert from fractional pixels to whole pixels.
    // TODO: Should we round the result rather than truncating the result?
    width = width / 64;

    return width;
}

void HeadlessContainer::draw_text(uintptr_t hdc,
    const char* text,
    uintptr_t hFont,
    litehtml::Color color,
    const litehtml::Position& pos)
{
    // Don't bother tracing this function as it's called once per token (where
    // a token is a discrete unit of text like a word).  See also text_width().
    //
    // That said, it can be useful to enable this function to see how litehtml
    // is tokenizing the text on the page.
    //
    // HEADLESS_TRACE1(HeadlessContainer::draw_text, text);

    OrionRenderContext* orc = reinterpret_cast<OrionRenderContext*>(hdc);
    Image<uint8_t>& canvas = orc->canvas;

    FT_Face face = (FT_Face)(hFont);
    FT_GlyphSlot glyph = face->glyph;

    // TODO: Should we round the result rather than truncating the result?
    int target_height = face->size->metrics.height / 64;

    FT_Vector pen;
    pen.x = pos.x * 64;
    pen.y = -pos.y * 64;

    // pos.x and pos.y represent the upper left corner where the text should render

    const char* end = text + strlen(text);
    const char* p = text;

    while (p != end) {
        char32_t c = utf8::next(p, end);

        // Stop rendering text if the pen falls outside the image bounds.
        // Otherwise FreeType may return a "raster overflow" error.  Given
        // that the FreeType documentation doesn't provide guidance on how to
        // deal with this error it's easiest to just avoid it all together.
        if (abs(pen.x) >= (canvas.width() * 64) || abs(pen.y) >= (canvas.height() * 64)) {
            break;
        }

        FT_Set_Transform(face, nullptr, &pen);
        FT_CALL(FT_Load_Char(face, c, FT_LOAD_RENDER));

        // target_height - glyph->bitmap_top effectively move the pen from the top
        // of the line to the bottom of the line then backtrack to where the glyph starts.

        draw_bitmap(canvas,
            &glyph->bitmap,
            color,
            glyph->bitmap_left,
            target_height - glyph->bitmap_top);

        pen.x += glyph->advance.x;
        pen.y += glyph->advance.y;
    }
}


int HeadlessContainer::pt_to_px(int pt)
{
    return (int)((double)pt * dpi_ / 72.0);
}

int HeadlessContainer::get_default_font_size() const
{
    return 16;
}

const char* HeadlessContainer::get_default_font_name() const
{
    return "Roboto";
}

void HeadlessContainer::load_image(const litehtml::URL& src, bool redraw_on_ready)
{
    HEADLESS_TRACE1(HeadlessContainer::load_image, src.string());

    std::string key = src.string();
    auto iterator = image_cache_.find(key);
    if (iterator != image_cache_.end()) {
        return;
    }

    http_response response = http_request(src);

    if (response.success()) {

        // TODO: Find a way to log responses to disk for further inspection
        // if necessary (e.g., to find out why our Image classes cannot decode
        // certain images).

        if (response.mime_type == "image/png") {
            image_cache_[key] = PNGCodec::decompress((uint8_t*)response.body.data(), response.body.length(), kImageFormatRGBA);
        } else if (response.mime_type == "image/jpeg") {
            image_cache_[key] = JPEGCodec::decompress((uint8_t*)response.body.data(), response.body.length(), kImageFormatRGBA);
        } else {
            image_cache_[key] = Image<uint8_t>(16, 16, kImageFormatRGBA);
        }
    }
}

litehtml::Size HeadlessContainer::get_image_size(const litehtml::URL& src)
{
    HEADLESS_TRACE1(HeadlessContainer::get_image_size, src.string());

    std::string key = src.string();
    auto iterator = image_cache_.find(key);
    if (iterator == image_cache_.end()) {
        return litehtml::Size();
    }

    return litehtml::Size(iterator->second.width(), iterator->second.height());
}

void HeadlessContainer::draw_background(uintptr_t hdc,
    const litehtml::BackgroundPaint& bg)
{
    HEADLESS_TRACE1(HeadlessContainer::draw_background, bg.image.string());

    OrionRenderContext* orc = reinterpret_cast<OrionRenderContext*>(hdc);
    Image<uint8_t>& canvas = orc->canvas;

    orion::rasterizer_scanline_aa<> ras;
    orion::scanline_p8 scanline;

    ras.auto_close(false);

    // Draw the background.
    orion::rounded_rect rect(
        bg.clip_box.x,
        bg.clip_box.y,
        bg.clip_box.x + bg.clip_box.width,
        bg.clip_box.y + bg.clip_box.height,
        0);

    rect.radius(
        bg.border_radii.top_left.x,
        bg.border_radii.top_left.y,
        bg.border_radii.top_right.x,
        bg.border_radii.top_right.y,
        bg.border_radii.bottom_right.x,
        bg.border_radii.bottom_right.y,
        bg.border_radii.bottom_left.x,
        bg.border_radii.bottom_left.y);

    ras.reset();
    ras.add_path(rect);

    const orion::rgba8 color = convert_color(bg.color);
    orion::render_scanlines_aa_solid(ras, scanline, orc->render_base, color);

    if (bg.image.empty()) {
        return;
    }

    std::string key = bg.image.string();
    auto iterator = image_cache_.find(key);
    if (iterator == image_cache_.end()) {
        // TODO: Return a "broken image" placeholder (as other browsers do).
        return;
    }

    // TODO: Check that all of the images are RGBA images.
    // TODO: Use composite() here to draw the background image.
    constexpr int channels = 4;

    Image<uint8_t>& image = iterator->second;

    // It's not clear how we should use the current clip_box variable when
    // rendering the image.  For example, sometimes the background position
    // variables (i.e., position_x and position_y) appear to be relative to
    // clip_box.  Other times they appear to be absolute.  Other container
    // implementations do not provide much guidance.

    // For now assume the positions are absolute, not relative, as that seems
    // to work well for most web pages.

    for (int y = 0; y < image.height(); y++) {
        const uint8_t* src_row = image.row(y);
        uint8_t* dst_row = canvas.row(y + bg.position_y) + (bg.position_x) * channels;
        for (int x = 0; x < image.width(); x++) {
            for (int c = 0; c < channels; c++) {
                *dst_row++ = *src_row++;
            }
        }
    }
}

class RoundedBorderPath {
    int x1_;
    int y1_;
    int x2_;
    int y2_;
    int rx1_;
    int ry1_;
    int rx2_;
    int ry2_;
    int rx3_;
    int ry3_;
    int rx4_;
    int ry4_;
    unsigned status_ = 0;
    orion::arc arc_;

public:
    RoundedBorderPath() = delete;

    RoundedBorderPath(const litehtml::Position& draw_position, const litehtml::BorderRadii& radii)
    : x1_(draw_position.x)
    , y1_(draw_position.y)
    , x2_(draw_position.x + draw_position.width)
    , y2_(draw_position.y + draw_position.height)
    , rx1_(radii.top_left.x)
    , ry1_(radii.top_left.y)
    , rx2_(radii.top_right.x)
    , ry2_(radii.top_right.y)
    , rx3_(radii.bottom_right.x)
    , ry3_(radii.bottom_right.y)
    , rx4_(radii.bottom_left.x)
    , ry4_(radii.bottom_left.y)
    {
    }

    void rewind(unsigned)
    {
        status_ = 0;
    }

    unsigned vertex(double* x, double* y)
    {
        unsigned cmd = orion::path_cmd_stop;

        switch (status_) {
            case 0:
                arc_.init(x1_ + rx1_, y1_ + ry1_, rx1_, ry1_, orion::pi, orion::pi + orion::pi * 0.5);
                arc_.rewind(0);
                status_++;

            case 1:
                cmd = arc_.vertex(x, y);
                if (orion::is_stop(cmd)) {
                    status_++;
                } else {
                    return cmd;
                }

            case 2:
                arc_.init(x2_ - rx2_, y1_ + ry2_, rx2_, ry2_, orion::pi + orion::pi * 0.5, 0.0);
                arc_.rewind(0);
                status_++;

            case 3:
                cmd = arc_.vertex(x, y);
                if (orion::is_stop(cmd)) {
                    status_++;
                } else {
                    return orion::path_cmd_line_to;
                }

            case 4:
                arc_.init(x2_ - rx3_, y2_ - ry3_, rx3_, ry3_, 0.0, orion::pi * 0.5);
                arc_.rewind(0);
                status_++;

            case 5:
                cmd = arc_.vertex(x, y);
                if (orion::is_stop(cmd)) {
                    status_++;
                } else {
                    return orion::path_cmd_line_to;
                }

            case 6:
                arc_.init(x1_ + rx4_, y2_ - ry4_, rx4_, ry4_, orion::pi * 0.5, orion::pi);
                arc_.rewind(0);
                status_++;

            case 7:
                cmd = arc_.vertex(x, y);
                if (orion::is_stop(cmd)) {
                    status_++;
                } else {
                    return orion::path_cmd_line_to;
                }

            case 8:
                cmd = orion::path_cmd_end_poly | orion::path_flags_close | orion::path_flags_ccw;
                status_++;
                break;
        }
        return cmd;
    }
};


void draw_border(OrionRenderContext* orc,
  orion::rasterizer_scanline_aa<>& ras,
  orion::scanline_p8& scanline,
  const litehtml::Border* border,
  const litehtml::Position& position)
{
    assert(border->style > kBorderStyleHidden);

    // draw_border() can only draw solid borders.
    assert(border->style == kBorderStyleSolid);

    const orion::rgba8 color = convert_color(border->color);

    Path path({
        position.x,
        position.y,
        position.x + position.width,
        position.y + position.height
    });

    orion::conv_stroke<Path> stroke_path(path);
    double stroke_width = border->width;
    stroke_path.width(stroke_width);
    stroke_path.line_cap(orion::square_cap);
    stroke_path.line_join(orion::miter_join);
    stroke_path.miter_limit(stroke_width);

    ras.reset();
    ras.add_path(stroke_path);
    orion::render_scanlines_aa_solid(ras, scanline, orc->render_base, color);
}

void HeadlessContainer::draw_borders(uintptr_t hdc,
    const litehtml::Borders& borders,
    const litehtml::Position& draw_position,
    bool root)
{
    // Don't bother tracing this function as it's called frequently.
    // HEADLESS_TRACE0(HeadlessContainer::draw_borders);

    OrionRenderContext* orc = reinterpret_cast<OrionRenderContext*>(hdc);

    orion::rasterizer_scanline_aa<> ras;
    ras.auto_close(false);

    orion::scanline_p8 scanline;

    if (borders.radii.has_radius()) {
        RoundedBorderPath path(draw_position, borders.radii);

        const litehtml::Border& border = borders.top;
        const orion::rgba8 color = convert_color(border.color);

        orion::conv_stroke<RoundedBorderPath> stroke_path(path);
        double stroke_width = border.width;
        stroke_path.width(stroke_width);
        stroke_path.line_cap(orion::square_cap);
        stroke_path.line_join(orion::miter_join);
        stroke_path.miter_limit(stroke_width);

        ras.reset();
        ras.add_path(stroke_path);
        orion::render_scanlines_aa_solid(ras, scanline, orc->render_base, color);
    } else {
        std::array<const litehtml::Border*, 4> b = {
            &borders.left,
            &borders.top,
            &borders.right,
            &borders.bottom
        };

        int x = draw_position.x;
        int y = draw_position.y;
        int width = draw_position.width;
        int height = draw_position.height;

        std::array<litehtml::Position, 4> c = {
            litehtml::Position(x, y, 0, height),
            litehtml::Position(x, y, width, 0),
            litehtml::Position(x + width, y, 0, height),
            litehtml::Position(x, y + height, width, 0),
        };

        for (int i = 0; i < 4; i++) {
            if (b[i]->style > kBorderStyleHidden && b[i]->width > 0) {
                draw_border(orc, ras, scanline, b[i], c[i]);
            }
        }
    }
}

void HeadlessContainer::draw_list_marker(uintptr_t hdc,
    const litehtml::list_marker& marker)
{
    HEADLESS_TRACE0(HeadlessContainer::draw_list_marker);
}

void HeadlessContainer::get_media_features(litehtml::MediaFeatures& media) const
{
    litehtml::Position client = get_client_rect();
    media.type = litehtml::kMediaTypeScreen;
    media.width = client.width;
    media.height = client.height;
    media.device_width = kDefaultWidth;
    media.device_height = kDefaultHeight;
    media.color = 8;
    media.monochrome = 0;
    media.color_index = 0;
    media.resolution = dpi_;
}

void HeadlessContainer::get_language(std::string& language,
    std::string& culture) const
{
    language = "en";
    culture = "";
}

void HeadlessContainer::link(const litehtml::Document* ptr,
    const litehtml::Element::ptr& el)
{
}

void HeadlessContainer::transform_text(std::string& text,
    litehtml::TextTransform tt)
{
    HEADLESS_TRACE0(HeadlessContainer::transform_text);
}

void HeadlessContainer::set_clip(const litehtml::Position& position,
    const litehtml::BorderRadii& border_radii,
    bool valid_x,
    bool valid_y)
{
    HEADLESS_TRACE0(HeadlessContainer::set_clip);
}

void HeadlessContainer::del_clip()
{
    HEADLESS_TRACE0(HeadlessContainer::del_clip);
}

void HeadlessContainer::set_caption(const char* caption)
{
}

void HeadlessContainer::on_anchor_click(const char* url,
    const litehtml::Element* el)
{
}

void HeadlessContainer::set_cursor(const char* cursor)
{
}

std::string HeadlessContainer::import_css(const litehtml::URL& url)
{
    HEADLESS_TRACE1(HeadlessContainer::import_css, url.string());

    http_response response = http_request(url);
    return response.body;
}

litehtml::Position HeadlessContainer::get_client_rect() const
{
    litehtml::Position client;

    client.width = kDefaultWidth;
    client.height = kDefaultHeight;

    return client;
}

} // namespace headless
