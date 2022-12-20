#include <string>

namespace utils
{
	std::string
	OmitExcessBytes(const std::string &s, std::size_t max_width, const std::string &ellipsis)
	{
		if (s.size() <= max_width) {
			return s;
		}
		return s.substr(0, max_width - ellipsis.size()) + ellipsis;
	}
} // namespace utils
