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

#include "litehtml/el_font.h"

namespace litehtml {

el_font::el_font(const std::shared_ptr<document>& doc) : html_tag(doc)
{

}

el_font::~el_font()
{

}

void el_font::parse_attributes()
{
	const tchar_t* str = get_attr(_t("color"));
	if(str)
	{
		m_style.add_property(_t("color"), str, 0, false);
	}

	str = get_attr(_t("face"));
	if(str)
	{
		m_style.add_property(_t("font-face"), str, 0, false);
	}

	str = get_attr(_t("size"));
	if(str)
	{
		int sz = t_atoi(str);
		if(sz <= 1)
		{
			m_style.add_property(_t("font-size"), _t("x-small"), 0, false);
		} else if(sz >= 6)
		{
			m_style.add_property(_t("font-size"), _t("xx-large"), 0, false);
		} else
		{
			switch(sz)
			{
			case 2:
				m_style.add_property(_t("font-size"), _t("small"), 0, false);
				break;
			case 3:
				m_style.add_property(_t("font-size"), _t("medium"), 0, false);
				break;
			case 4:
				m_style.add_property(_t("font-size"), _t("large"), 0, false);
				break;
			case 5:
				m_style.add_property(_t("font-size"), _t("x-large"), 0, false);
				break;
			}
		}
	}

	html_tag::parse_attributes();
}

} // namespace litehtml
