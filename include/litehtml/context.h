#ifndef LH_CONTEXT_H
#define LH_CONTEXT_H

#include "litehtml/css_stylesheet.h"

namespace litehtml
{
	class context
	{
		litehtml::css_stylesheet m_master_css;
	public:
		void			load_master_stylesheet(const tchar_t* str);
		litehtml::css_stylesheet&	master_css()
		{
			return m_master_css;
		}
	};
}

#endif  // LH_CONTEXT_H
