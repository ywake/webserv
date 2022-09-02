#include "parse_path.hpp"

#include "ThinString.hpp"
#include "parse_define.hpp"
#include "parse_path_relative.hpp"
#include "parse_path_utils.hpp"
#include "parse_uri_utils.hpp"

namespace ABNF
{
	static bool IsValidPath(const ThinString &str);

	bool IsPathAbempty(const ThinString &str)
	{
		if (str.empty()) {
			return true;
		}
		return IsPathAbsolute(str);
	}

	// absolute-path = "/" [ segment-nz *( "/" segment ) ] -> start with "//"
	// absolute-path = 1 *( "/" segment ) -> start with "//" ok
	bool IsPathAbsolute(const ThinString &str)
	{
		if (str.empty() || str.at(0) != '/') {
			return false;
		}
		return IsValidPath(str);
	}

	// path-rootless = segment-nz *( "/" segment )
	bool IsPathRootless(const ThinString &str)
	{
		if (str.empty() || str.at(0) == '/') {
			return false;
		}
		return IsValidPath(str);
	}

	// path-empty = 0<pchar>
	bool IsPathEmpty(const ThinString &str)
	{
		return str.empty();
	}

	static bool IsValidPath(const ThinString &str)
	{
		StringAry tokens = TokenizePath(str);
		for (StringAry::const_iterator itr = tokens.begin(); itr != tokens.end(); itr++) {
			if (*itr == "/") {
				continue;
			}
			if (!IsSegment(*itr)) {
				return false;
			}
		}
		return true;
	}
} // namespace ABNF
