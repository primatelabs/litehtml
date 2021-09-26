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

#include "litehtml/iterators.h"

#include "litehtml/html.h"
#include "litehtml/html_tag.h"

litehtml::element::ptr litehtml::elements_iterator::next(bool ret_parent)
{
	next_idx();

	while(m_idx < (int) m_el->get_children_count())
	{
		element::ptr el = m_el->get_child(m_idx);
		if(	el->get_children_count() && m_go_inside && m_go_inside->select(el) )
		{
			stack_item si;
			si.idx		= m_idx;
			si.el		= m_el;
			m_stack.push_back(si);
			m_el		= el;
			m_idx		= -1;
			if(ret_parent)
			{
				return el;
			}
			next_idx();
		} else
		{
			if( !m_select || (m_select && m_select->select(m_el->get_child(m_idx))) )
			{
				return m_el->get_child(m_idx);
			} else
			{
				next_idx();
			}
		}
	}

	return 0;
}

void litehtml::elements_iterator::next_idx()
{
	m_idx++;
	while(m_idx >= (int) m_el->get_children_count() && m_stack.size())
	{
		stack_item si = m_stack.back();
		m_stack.pop_back();
		m_idx	= si.idx;
		m_el	= si.el;
		m_idx++;
		continue;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


bool litehtml::go_inside_inline::select(const element::ptr& el)
{
	if(el->get_display() == display_inline || el->get_display() == display_inline_text)
	{
		return true;
	}
	return false;
}

bool litehtml::go_inside_table::select(const element::ptr& el)
{
	if(	el->get_display() == display_table_row_group ||
		el->get_display() == display_table_header_group ||
		el->get_display() == display_table_footer_group)
	{
		return true;
	}
	return false;
}

bool litehtml::table_rows_selector::select(const element::ptr& el)
{
	if(	el->get_display() == display_table_row)
	{
		return true;
	}
	return false;
}

bool litehtml::table_cells_selector::select(const element::ptr& el)
{
	if(	el->get_display() == display_table_cell)
	{
		return true;
	}
	return false;
}
