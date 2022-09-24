#include <string>

namespace utils
{
	std::string ToLowerString(std::string s)
	{
		for (std::string::iterator it = s.begin(); it != s.end(); it++) {
			*it = std::tolower(*it);
		}
		return s;
	}
} // namespace utils
