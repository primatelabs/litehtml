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

#include "litehtml/el_link.h"

#include "litehtml/document.h"
#include "litehtml/document_container.h"

litehtml::el_link::el_link(const std::shared_ptr<litehtml::document>& doc) : litehtml::html_tag(doc)
{

}

litehtml::el_link::~el_link()
{

}

void litehtml::el_link::parse_attributes()
{
	bool processed = false;

	document::ptr doc = get_document();

	const tchar_t* rel = get_attr(_t("rel"));
	if(rel && !t_strcmp(rel, _t("stylesheet")))
	{
		const tchar_t* media	= get_attr(_t("media"));
		const tchar_t* href		= get_attr(_t("href"));
		if(href && href[0])
		{
			tstring css_text;
			tstring css_baseurl;
			doc->container()->import_css(css_text, href, css_baseurl);
			if(!css_text.empty())
			{
				doc->add_stylesheet(css_text.c_str(), css_baseurl.c_str(), media);
				processed = true;
			}
		}
	}

	if(!processed)
	{
		doc->container()->link(doc, shared_from_this());
	}
}
