#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include "result.hpp"
#include "thin_string.hpp"
#include <string>
#include <vector>

std::vector<ThinString> Split(const ThinString &str, const std::string delim);
Result<long>            StrToLongDenyPadZero(const std::string &str);

// TODO 全部 util
namespace utils
{
	std::string  ToLowerString(std::string s);
	Result<long> StrToLong(const std::string &str);

} // namespace utils

#endif
