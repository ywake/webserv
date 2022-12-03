#include <deque>
#include <vector>

#include "thin_string.hpp"
#include "webserv_utils.hpp"

namespace utils
{
	typedef std::vector<ThinString> Strings;

	ThinString NromalizePath(const ThinString &path)
	{
			(void)path;
			return "/";
	}
} // namespace utils
