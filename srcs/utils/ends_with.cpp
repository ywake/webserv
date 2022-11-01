#include "webserv_utils.hpp"

#include <algorithm>
#include <string>

bool EndsWith(const std::string &str, const std::string &suffix)
{
	if (str.size() < suffix.size()) {
		return false;
	}
	return std::equal(std::rbegin(suffix), std::rend(suffix), std::rbegin(str));
}
