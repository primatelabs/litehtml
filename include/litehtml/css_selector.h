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

#include "style.h"
#include "media_query.h"

namespace litehtml {

	//////////////////////////////////////////////////////////////////////////

	struct selector_specificity
	{
		int		a;
		int		b;
		int		c;
		int		d;

		selector_specificity(int va = 0, int vb = 0, int vc = 0, int vd = 0)
		{
			a	= va;
			b	= vb;
			c	= vc;
			d	= vd;
		}

		void operator += (const selector_specificity& val)
		{
			a	+= val.a;
			b	+= val.b;
			c	+= val.c;
			d	+= val.d;
		}

		bool operator==(const selector_specificity& val) const
		{
			if(a == val.a && b == val.b && c == val.c && d == val.d)
			{
				return true;
			}
			return false;
		}

		bool operator!=(const selector_specificity& val) const
		{
			if(a != val.a || b != val.b || c != val.c || d != val.d)
			{
				return true;
			}
			return false;
		}

		bool operator > (const selector_specificity& val) const
		{
			if(a > val.a)
			{
				return true;
			} else if(a < val.a)
			{
				return false;
			} else
			{
				if(b > val.b)
				{
					return true;
				} else if(b < val.b)
				{
					return false;
				} else
				{
					if(c > val.c)
					{
						return true;
					} else if(c < val.c)
					{
						return false;
					} else
					{
						if(d > val.d)
						{
							return true;
						} else if(d < val.d)
						{
							return false;
						}
					}
				}
			}
			return false;
		}

		bool operator >= (const selector_specificity& val) const
		{
			if((*this) == val) return true;
			if((*this) > val) return true;
			return false;
		}

		bool operator <= (const selector_specificity& val) const
		{
			if((*this) > val)
			{
				return false;
			}
			return true;
		}

		bool operator < (const selector_specificity& val) const
		{
			if((*this) <= val && (*this) != val)
			{
				return true;
			}
			return false;
		}

	};

	//////////////////////////////////////////////////////////////////////////

	enum attr_select_condition
	{
		select_exists,
		select_equal,
		select_contain_str,
		select_start_str,
		select_end_str,
		select_pseudo_class,
		select_pseudo_element,
	};

	//////////////////////////////////////////////////////////////////////////

	struct css_attribute_selector
	{
		typedef std::vector<css_attribute_selector>	vector;

		tstring					attribute;
		tstring					val;
		string_vector			class_val;
		attr_select_condition	condition;

		css_attribute_selector()
		{
			condition = select_exists;
		}
	};

	//////////////////////////////////////////////////////////////////////////

	class css_element_selector
	{
	public:
		tstring							m_tag;
		css_attribute_selector::vector	m_attrs;
	public:

		void parse(const tstring& txt);
	};

	//////////////////////////////////////////////////////////////////////////

	enum css_combinator
	{
		combinator_descendant,
		combinator_child,
		combinator_adjacent_sibling,
		combinator_general_sibling
	};

	//////////////////////////////////////////////////////////////////////////

	class css_selector
	{
	public:
		typedef std::shared_ptr<css_selector>	ptr;
		typedef std::vector<css_selector::ptr>	vector;

	public:
		selector_specificity	m_specificity;
		css_element_selector	m_right;
		css_selector::ptr		m_left;
		css_combinator			m_combinator;
		style::ptr				m_style;
		int						m_order;
		media_query_list::ptr	m_media_query;

	public:
		css_selector(media_query_list::ptr media)
		{
			m_media_query	= media;
			m_combinator	= combinator_descendant;
			m_order			= 0;
		}

		~css_selector()
		{
		}

		css_selector(const css_selector& val)
		{
			m_right			= val.m_right;
			if(val.m_left)
			{
				m_left			= std::make_shared<css_selector>(*val.m_left);
			} else
			{
				m_left = 0;
			}
			m_combinator	= val.m_combinator;
			m_specificity	= val.m_specificity;
			m_order			= val.m_order;
			m_media_query	= val.m_media_query;
		}

		bool parse(const tstring& text);
		void calc_specificity();
		bool is_media_valid() const;
		void add_media_to_doc(document* doc) const;
	};

	inline bool css_selector::is_media_valid() const
	{
		if(!m_media_query)
		{
			return true;
		}
		return m_media_query->is_used();
	}


	//////////////////////////////////////////////////////////////////////////

	inline bool operator > (const css_selector& v1, const css_selector& v2)
	{
		if(v1.m_specificity == v2.m_specificity)
		{
			return (v1.m_order > v2.m_order);
		}
		return (v1.m_specificity > v2.m_specificity);
	}

	inline bool operator < (const css_selector& v1, const css_selector& v2)
	{
		if(v1.m_specificity == v2.m_specificity)
		{
			return (v1.m_order < v2.m_order);
		}
		return (v1.m_specificity < v2.m_specificity);
	}

	inline bool operator >(const css_selector::ptr& v1, const css_selector::ptr& v2)
	{
		return (*v1 > *v2);
	}

	inline bool operator < (const css_selector::ptr& v1, const css_selector::ptr& v2)
	{
		return (*v1 < *v2);
	}

	//////////////////////////////////////////////////////////////////////////

	class used_selector
	{
	public:
		typedef std::unique_ptr<used_selector>	ptr;
		typedef std::vector<used_selector::ptr>	vector;

		css_selector::ptr	m_selector;
		bool				m_used;

		used_selector(const css_selector::ptr& selector, bool used)
		{
			m_used		= used;
			m_selector	= selector;
		}
	};

} // namespace litehtml

#endif  // LITEHTML_CSS_SELECTOR_H__
