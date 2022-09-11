#include "parse_uri_utils.hpp"

#include "parse_abnf_core_rules.hpp"
#include "parse_define.hpp"
#include "parse_uri_relative.hpp"
#include "thin_string.hpp"
#include <cstring>
// TODO hexdigitAllとかここにあっていいのか微妙
// TODO ~all系の命名が微妙かも
namespace ABNF
{
	// unreserved / pct-encoded / sub-delims
	bool IsRegularUriChar(const char c, const char *additional_char_set)
	{
		if (c == '\0') {
			return false;
		}
		return IsUnreserved(c) || IsSubDelims(c) || std::strchr(additional_char_set, c);
	}

	bool IsRegularUriCharOnly(const ThinString &str, const char *additional_char_set)
	{
		if (str.empty()) {
			return false;
		}
		for (ThinString::const_iterator itr = str.begin(); itr != str.end(); itr++) {
			if (!IsRegularUriChar(*itr, additional_char_set)) {
				return false;
			}
		}
		return true;
	}

	bool IsRegularUriToken(const ThinString &token, const char *additional_char_set)
	{
		if (token.size() == kPctEncodingSize) {
			return IsPctEncoded(token);
		} else if (token.size() == sizeof(char)) {
			char c = *token.begin();
			return IsRegularUriChar(c, additional_char_set);
		} else {
			return false;
		}
	}

	bool IsRegularUriTokenOnly(const StringAry &tokens, const char *additional_char_set)
	{
		for (Iterator it = tokens.begin(); it != tokens.end(); it++) {
			if (!IsRegularUriToken(*it, additional_char_set)) {
				return false;
			}
		}
		return true;
	}

} // namespace ABNF
