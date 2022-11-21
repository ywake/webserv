#include <string>

namespace utils
{
	bool EndWith(const std::string &src, const std::string &target)
	{
		typedef std::string::const_iterator Iterator;
		if (src.size() < target.size()) {
			return false;
		}
		for (Iterator src_it = src.end() - target.size(), target_it = target.begin();
			 src_it != src.end();
			 src_it++, target_it++) {
			if (*src_it != *target_it) {
				return false;
			}
		}
		return true;
	}

} // namespace utils
