#include "parse_uri_utils.hpp"

#include "ThinString.hpp"
#include "parse_abnf_core_rules.hpp"
#include "parse_define.hpp"
#include "parse_uri_relative.hpp"
#include <cstring>

namespace ABNF
{
	// unreserved / pct-encoded / sub-delims
	bool IsRegularURIChar(const char c, const char *additional_char_set)
	{
		return IsUnreserved(c) || IsSubDelims(c) || std::strchr(additional_char_set, c);
	}

	bool IsRegularURIToken(const ThinString &token, const char *additional_char_set)
	{
		if (token.size() == kPctEncodingSize) {
			return IsPctEncoded(token);
		} else if (token.size() == sizeof(char)) {
			char c = *token.begin();
			return IsRegularURIChar(c, additional_char_set);
		} else {
			return false;
		}
	}

	bool IsHexDigitAll(const ThinString &str)
	{
		for (ThinString::const_iterator itr = str.begin(); itr != str.end(); itr++) {
			if (!IsHexDigit(*itr)) {
				return false;
			}
		}
		return true;
	}
} // namespace ABNF
