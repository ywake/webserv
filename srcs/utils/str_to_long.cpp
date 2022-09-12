#include "webserv_utils.hpp"
#include <cerrno>

Result<long> StrToLong(const std::string &str)
{
	if (str.empty()) {
		return false;
	}
	bool has_zero_padding = str.at(0) == '0' && str.length() > 2;
	if (has_zero_padding) {
		return Error("");
	}
	errno            = 0;
	char *endptr     = NULL;
	long  n          = std::strtol(str.c_str(), &endptr, 10);
	bool  is_all_num = *endptr == '\0';
	if (!is_all_num || errno) {
		return Error("");
	}
	return n;
}
