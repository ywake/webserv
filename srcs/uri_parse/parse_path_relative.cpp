#include "parse_path_relative.hpp"

#include "parse_path_utils.hpp"
#include "parse_uri_utils.hpp"
#include "thin_string.hpp"

// const char *にするとsizeofで長さが取れないのでdefineする
#define PCHAR_UNIQ_SET ":@"

namespace ABNF
{
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
		return IsRegularUriToken(token, PCHAR_UNIQ_SET);
	}

} // namespace ABNF
