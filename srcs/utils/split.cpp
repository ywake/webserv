#include "webserv_utils.hpp"

#include <vector>

//[FIX]
std::vector<ThinString> Split(const ThinString &str, const std::string delim)
{
	std::vector<ThinString> split;
	std::size_t				delim_length = delim.length();
	if (delim_length == 0) {
		split.push_back(str);
		return split;
	}
	std::size_t offset = 0;
	while (true) {
		std::size_t pos = str.find(delim, offset);
		if (pos == std::string::npos) {
			split.push_back(str.substr(offset));
			break;
		}
		split.push_back(str.substr(offset, pos - offset));
		offset = pos + delim_length;
	}
	return split;
}
