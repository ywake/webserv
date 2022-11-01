#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include "result.hpp"
#include "thin_string.hpp"
#include <string>
#include <vector>

std::vector<ThinString> Split(const ThinString &str, const std::string delim);
Result<long>            StrToLongDenyPadZero(const std::string &str);
bool                    IsRegularFile(const std::string &path);
bool                    EndsWith(const std::string &str, const std::string &suffix);

// TODO 全部 util
namespace utils
{
	template <typename T>
	void DeleteSafe(T *&p)
	{
		delete p;
		p = NULL;
	}
	std::string  ToLowerString(std::string s);
	Result<long> StrToLong(const std::string &str);

} // namespace utils

#endif
