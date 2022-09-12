#ifndef __PARSE_URI_H__
#define __PARSE_URI_H__

#include "parse_authority.hpp"
#include "parse_define.hpp"
#include "parse_path.hpp"
#include "parse_path_relative.hpp"
#include "parse_path_utils.hpp"
#include "thin_string.hpp"

namespace ABNF
{
	static const ThinString &kSchemeUniqSet = "+-.";
	static const ThinString &kQueryUniqSet  = "/?";

	// scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
	bool IsScheme(const ThinString &str)
	{
		if (str.empty() || !std::isalpha(str.at(0))) {
			return false;
		}
		for (ThinString::const_iterator it = str.begin(); it != str.end(); it++) {
			if (!std::isalnum(*it) && kSchemeUniqSet.find(*it) == ThinString::npos) {
				return false;
			}
		}
		return true;
	}

	// query         = *( pchar / "/" / "?" )
	bool IsQuery(const ThinString &str)
	{
		StringAry tokens = TokenizePchar(str);
		for (StringAry::const_iterator itr = tokens.begin(); itr != tokens.end(); itr++) {
			bool is_valid_token =
				IsPchar(*itr) || kQueryUniqSet.find(itr->at(0)) != ThinString::npos;
			if (!is_valid_token) {
				return false;
			}
		}
		return true;
	}

} // namespace ABNF

#endif /* __PARSE_URI_H__ */
