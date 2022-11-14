#include "thin_string.hpp"
#include <vector>

namespace utils
{
	std::vector<ThinString> TrimEmpty(const std::vector<ThinString> &vec)
	{
		std::vector<ThinString> trimmed;
		for (std::vector<ThinString>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
			if (!it->empty()) {
				trimmed.push_back(*it);
			}
		}
		return trimmed;
	}
} // namespace utils
