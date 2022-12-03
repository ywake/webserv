#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include "result.hpp"
#include "thin_string.hpp"
#include <string>
#include <vector>

std::vector<ThinString> Split(const ThinString &str, const std::string delim);
Result<long>            StrToLongDenyPadZero(const std::string &str);

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

// TODO 全部 util
namespace utils
{
	template <typename T>
	void DeleteSafe(T *&p)
	{
		delete p;
		p = NULL;
	}
	std::string             ToLowerString(std::string s);
	Result<long>            StrToLong(const std::string &str);
	Result<bool>            IsRegularFile(int fd);
	Result<std::string>     ReadFile(const std::string &file_path);
	Result<std::size_t>     StrToUnsignedLong(const std::string &str);
	std::vector<ThinString> TrimEmpty(const std::vector<ThinString> &vec);
	bool                    EndWith(const std::string &src, const std::string &target);
	void                    Close(int fd);
	std::string             NromalizePath(const ThinString &path);
} // namespace utils

#endif
