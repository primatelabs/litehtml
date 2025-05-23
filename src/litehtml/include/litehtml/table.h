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

#ifndef LITEHTML_TABLE_H__
#define LITEHTML_TABLE_H__

#include <vector>

#include "litehtml/css/css_length.h"
#include "litehtml/element/element.h"

namespace litehtml {
struct table_row {
    typedef std::vector<table_row> vector;

    int height;
    int border_top;
    int border_bottom;
    Element::ptr el_row;
    int top;
    int bottom;
    CSSLength css_height;
    int min_height;

    table_row()
    {
        min_height = 0;
        top = 0;
        bottom = 0;
        border_bottom = 0;
        border_top = 0;
        height = 0;
        el_row = nullptr;
        css_height.predef(0);
    }

    table_row(int h, Element::ptr& row)
    {
        min_height = 0;
        height = h;
        el_row = row;
        border_bottom = 0;
        border_top = 0;
        top = 0;
        bottom = 0;
        if (row) {
            css_height = row->get_css_height();
        }
    }

    table_row(const table_row& val)
    {
        min_height = val.min_height;
        top = val.top;
        bottom = val.bottom;
        border_bottom = val.border_bottom;
        border_top = val.border_top;
        height = val.height;
        css_height = val.css_height;
        el_row = val.el_row;
    }

    table_row(table_row&& val)
    {
        min_height = val.min_height;
        top = val.top;
        bottom = val.bottom;
        border_bottom = val.border_bottom;
        border_top = val.border_top;
        height = val.height;
        css_height = val.css_height;
        el_row = std::move(val.el_row);
    }
};

struct table_column {
    typedef std::vector<table_column> vector;

    int min_width;
    int max_width;
    int width;
    CSSLength css_width;
    int border_left;
    int border_right;
    int left;
    int right;

    table_column()
    {
        left = 0;
        right = 0;
        border_left = 0;
        border_right = 0;
        min_width = 0;
        max_width = 0;
        width = 0;
        css_width.predef(0);
    }

    table_column(int min_w, int max_w)
    {
        left = 0;
        right = 0;
        border_left = 0;
        border_right = 0;
        max_width = max_w;
        min_width = min_w;
        width = 0;
        css_width.predef(0);
    }

    table_column(const table_column& val)
    {
        left = val.left;
        right = val.right;
        border_left = val.border_left;
        border_right = val.border_right;
        max_width = val.max_width;
        min_width = val.min_width;
        width = val.width;
        css_width = val.css_width;
    }
};

class table_column_accessor {
public:
    virtual ~table_column_accessor();
    virtual int& get(table_column& col) = 0;
};

class table_column_accessor_max_width : public table_column_accessor {
public:
    virtual int& get(table_column& col);
};

class table_column_accessor_min_width : public table_column_accessor {
public:
    virtual int& get(table_column& col);
};

class table_column_accessor_width : public table_column_accessor {
public:
    virtual int& get(table_column& col);
};

struct table_cell {
    Element::ptr el;
    int colspan;
    int rowspan;
    int min_width;
    int min_height;
    int max_width;
    int max_height;
    int width;
    int height;
    Margins borders;

    table_cell()
    {
        min_width = 0;
        min_height = 0;
        max_width = 0;
        max_height = 0;
        width = 0;
        height = 0;
        colspan = 1;
        rowspan = 1;
        el = nullptr;
    }

    table_cell(const table_cell& val)
    {
        el = val.el;
        colspan = val.colspan;
        rowspan = val.rowspan;
        width = val.width;
        height = val.height;
        min_width = val.min_width;
        min_height = val.min_height;
        max_width = val.max_width;
        max_height = val.max_height;
        borders = val.borders;
    }

    table_cell(const table_cell&& val)
    {
        el = std::move(val.el);
        colspan = val.colspan;
        rowspan = val.rowspan;
        width = val.width;
        height = val.height;
        min_width = val.min_width;
        min_height = val.min_height;
        max_width = val.max_width;
        max_height = val.max_height;
        borders = val.borders;
    }
};

class table_grid {
public:
    typedef std::vector<std::vector<table_cell>> rows;

private:
    int m_rows_count;
    int m_cols_count;
    rows m_cells;
    table_column::vector m_columns;
    table_row::vector m_rows;

public:
    table_grid()
    {
        m_rows_count = 0;
        m_cols_count = 0;
    }

    void clear();
    void begin_row(Element::ptr& row);
    void add_cell(Element::ptr& el);
    bool is_rowspanned(int r, int c);
    void finish();
    table_cell* cell(int t_col, int t_row);
    table_column& column(int c)
    {
        return m_columns[c];
    }
    table_row& row(int r)
    {
        return m_rows[r];
    }

    int rows_count()
    {
        return m_rows_count;
    }
    int cols_count()
    {
        return m_cols_count;
    }

    void distribute_max_width(int width, int start, int end);
    void distribute_min_width(int width, int start, int end);
    void distribute_width(int width, int start, int end);
    void distribute_width(int width, int start, int end, table_column_accessor* acc);
    int calc_table_width(int block_width,
        bool is_auto,
        int& min_table_width,
        int& max_table_width);
    void calc_horizontal_positions(Margins& table_borders,
        border_collapse bc,
        int bdr_space_x);
    void calc_vertical_positions(Margins& table_borders,
        border_collapse bc,
        int bdr_space_y);
    void calc_rows_height(int blockHeight, int borderSpacingY);
};
} // namespace litehtml

#endif // LITEHTML_TABLE_H__
