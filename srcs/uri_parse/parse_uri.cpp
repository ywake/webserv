#ifndef __PARSE_URI_H__
#define __PARSE_URI_H__

#include "parse_authority.hpp"
#include "parse_define.hpp"
#include "parse_path.hpp"
#include "parse_path_relative.hpp"
#include "parse_path_utils.hpp"
#include "thin_string.hpp"
#include <cstring>

// const char *にするとsizeofで長さが取れないのでdefineする
#define SCHEME_UNIQ_SET "+-."
#define QUERY_UNIQ_SET "/?"

namespace ABNF
{

	// scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
	bool IsScheme(const ThinString &str)
	{
		if (str.empty() || !std::isalpha(str.at(0))) {
			return false;
		}
		for (ThinString::const_iterator it = str.begin(); it != str.end(); it++) {
			if (!std::isalnum(*it) &&
				!std::memchr(SCHEME_UNIQ_SET, *it, sizeof(SCHEME_UNIQ_SET) - 1)) {
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
				IsPchar(*itr) ||
				std::memchr(QUERY_UNIQ_SET, itr->at(0), sizeof(QUERY_UNIQ_SET) - 1);
			if (!is_valid_token) {
				return false;
			}
		}
		return true;
	}

} // namespace ABNF

#endif /* __PARSE_URI_H__ */
