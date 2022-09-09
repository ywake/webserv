#include "parse_path_relative.hpp"

#include "parse_path_utils.hpp"
#include "parse_uri_utils.hpp"
#include "thin_string.hpp"

namespace ABNF
{

	static const char *kPcharUniqSet = ":@";

	// segment       = *pchar
	bool IsSegment(const ThinString &str)
	{
		StringAry token = TokenizePchar(str);
		for (StringAry::const_iterator itr = token.begin(); itr != token.end(); itr++) {
			if (!IsPchar(*itr)) {
				return false;
			}
		}
		return true;
	}

	// pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
	bool IsPchar(const ThinString &token)
	{
		return IsRegularURIToken(token, kPcharUniqSet);
	}

} // namespace ABNF
