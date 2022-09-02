#ifndef URI_HPP
#define URI_HPP

#include "ThinString.hpp"
#include "parse_authority.hpp"
#include "parse_define.hpp"
#include "parse_path.hpp"
#include "parse_path_relative.hpp"
#include "parse_path_utils.hpp"
#include <cstring>

namespace ABNF
{
	static const char *kSchemeUniqSet = "+-.";
	static const char *kQueryUniqSet = "/?";

	// scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
	bool IsScheme(const ThinString &str)
	{
		if (str.len() == 0 || !std::isalpha(str.at(0))) {
			return false;
		}
		for (ThinString::const_iterator it = str.begin(); it != str.end(); it++) {
			if (!std::isalnum(*it) && !std::strchr(kSchemeUniqSet, *it)) {
				return false;
			}
		}
		return true;
	}

	// hier-part    = "//" authority path-abempty
	//              / path-absolute
	//              / path-rootless ; Not support
	//              / path-empty
	bool IsHierPart(const ThinString &str)
	{
		if (IsPathEmpty(str)) {
			return true;
		} else if (str.find("//") == 0) {
			ThinString after_2slash = str.substr(2);
			ThinString::ThinStrPair authority_path =
				after_2slash.DivideBy("/", ThinString::kIncludeRight);
			return IsAuthority(authority_path.first) && IsPathAbempty(authority_path.second);
		} else if (IsPathAbsolute(str)) {
			return true;
		}
		return false;
	}

	// query         = *( pchar / "/" / "?" )
	bool IsQuery(const ThinString &str)
	{
		StringAry tokens = TokenizePchar(str);
		for (StringAry::const_iterator itr = tokens.begin(); itr != tokens.end(); itr++) {
			bool is_valid_token = IsPchar(*itr) || std::strchr(kQueryUniqSet, itr->at(0));
			if (!is_valid_token) {
				return false;
			}
		}
		return true;
	}

} // namespace ABNF

#endif
