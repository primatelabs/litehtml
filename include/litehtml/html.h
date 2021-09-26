#ifndef LH_HTML_H
#define LH_HTML_H

#include <stdlib.h>
#include <string>
#include <ctype.h>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <sstream>
#include "os_types.h"
#include "types.h"
#include "background.h"
#include "borders.h"
#include "html_tag.h"
#include "web_color.h"
#include "media_query.h"

namespace litehtml
{
	void trim(tstring &s);
	void lcase(tstring &s);
	int	 value_index(const tstring& val, const tstring& strings, int defValue = -1, tchar_t delim = _t(';'));
	bool value_in_list(const tstring& val, const tstring& strings, tchar_t delim = _t(';'));
	tstring::size_type find_close_bracket(const tstring &s, tstring::size_type off, tchar_t open_b = _t('('), tchar_t close_b = _t(')'));
	void split_string(const tstring& str, string_vector& tokens, const tstring& delims, const tstring& delims_preserve = _t(""), const tstring& quote = _t("\""));
	void join_string(tstring& str, const string_vector& tokens, const tstring& delims);

	inline int round_f(float val)
	{
		int int_val = (int) val;
		if(val - int_val >= 0.5)
		{
			int_val++;
		}
		return int_val;
	}

	inline int round_d(double val)
	{
		int int_val = (int) val;
		if(val - int_val >= 0.5)
		{
			int_val++;
		}
		return int_val;
	}
}

#endif  // LH_HTML_H
