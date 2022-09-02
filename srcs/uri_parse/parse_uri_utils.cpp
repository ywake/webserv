#include "parse_uri_utils.hpp"

#include "ThinString.hpp"
#include "parse_define.hpp"
#include "parse_uri_relative.hpp"
#include <cstring>

namespace ABNF
{
	// unreserved / pct-encoded / sub-delims
	bool IsValidURIChar(const ThinString &token, const char *additional_char_set)
	{
		if (token.size() == kPctEncodingSize) {
			return IsPctEncoded(token);
		} else if (token.size() == sizeof(char)) {
			char c = *token.begin();
			return IsUnreserved(c) || IsSubDelims(c) || std::strchr(additional_char_set, c);
		} else {
			return false;
		}
	}

} // namespace ABNF
