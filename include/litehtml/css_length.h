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

#ifndef LITEHTML_CSS_LENGTH_H__
#define LITEHTML_CSS_LENGTH_H__
#include "types.h"

namespace litehtml
{
	class css_length
	{
		union
		{
			float	m_value;
			int		m_predef;
		};
		css_units	m_units;
		bool		m_is_predefined;
	public:
		css_length();
		css_length(const css_length& val);

		css_length&	operator=(const css_length& val);
		css_length&	operator=(float val);
		bool		is_predefined() const;
		void		predef(int val);
		int			predef() const;
		void		set_value(float val, css_units units);
		float		val() const;
		css_units	units() const;
		int			calc_percent(int width) const;
		void		fromString(const tstring& str, const tstring& predefs = _t(""), int defValue = 0);
	};

	// css_length inlines

	inline css_length::css_length()
	{
		m_value			= 0;
		m_predef		= 0;
		m_units			= css_units_none;
		m_is_predefined	= false;
	}

	inline css_length::css_length(const css_length& val)
	{
		if(val.is_predefined())
		{
			m_predef	= val.m_predef;
		} else
		{
			m_value		= val.m_value;
		}
		m_units			= val.m_units;
		m_is_predefined	= val.m_is_predefined;
	}

	inline css_length&	css_length::operator=(const css_length& val)
	{
		if(val.is_predefined())
		{
			m_predef	= val.m_predef;
		} else
		{
			m_value		= val.m_value;
		}
		m_units			= val.m_units;
		m_is_predefined	= val.m_is_predefined;
		return *this;
	}

	inline css_length&	css_length::operator=(float val)
	{
		m_value = val;
		m_units = css_units_px;
		m_is_predefined = false;
		return *this;
	}

	inline bool css_length::is_predefined() const
	{
		return m_is_predefined;
	}

	inline void css_length::predef(int val)
	{
		m_predef		= val;
		m_is_predefined = true;
	}

	inline int css_length::predef() const
	{
		if(m_is_predefined)
		{
			return m_predef;
		}
		return 0;
	}

	inline void css_length::set_value(float val, css_units units)
	{
		m_value			= val;
		m_is_predefined = false;
		m_units			= units;
	}

	inline float css_length::val() const
	{
		if(!m_is_predefined)
		{
			return m_value;
		}
		return 0;
	}

	inline css_units css_length::units() const
	{
		return m_units;
	}

	inline int css_length::calc_percent(int width) const
	{
		if(!is_predefined())
		{
			if(units() == css_units_percentage)
			{
				return (int) ((double) width * (double) m_value / 100.0);
			} else
			{
				return (int) val();
			}
		}
		return 0;
	}
}

#endif  // LITEHTML_CSS_LENGTH_H__