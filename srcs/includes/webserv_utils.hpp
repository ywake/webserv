#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include "result.hpp"
#include "thin_string.hpp"
#include <csignal>
#include <sstream>
#include <string>
#include <vector>

#include "status_code.hpp"

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
	template <typename T>
	std::string ToString(const T &data)
	{
		std::stringstream ss;
		ss << data;
		return ss.str();
	}

	Result<void>            SetSignalHandler(int signum, sig_t handler, int sa_flags);
	Result<std::string>     CreateCurrentTimeStamp();
	std::string             ToLowerString(std::string s);
	Result<long>            StrToLong(const std::string &str);
	Result<bool>            IsRegularFile(int fd);
	Result<std::string>     ReadFile(const std::string &file_path);
	Result<std::size_t>     StrToUnsignedLong(const std::string &str);
	std::vector<ThinString> TrimEmpty(const std::vector<ThinString> &vec);
	bool                    EndWith(const std::string &src, const std::string &target);
	void                    Close(int fd);
	Result<std::string>     NormalizePath(const ThinString &path);
	std::string             JoinPath(const std::string &parent, const std::string &child);
	bool                    IsReadablePath(const std::string &path);
	bool                    IsWritablePath(const std::string &path);
	std::string             GetDirName(const std::string &path);

	std::string CreateDefaultPage(const http::StatusCode &code);
	std::string
	OmitExcessBytes(const std::string &s, std::size_t max_width, const std::string &ellipsis);

	// string utils
	char                               &GetLastChar(std::string str);
	std::pair<std::string, std::string> DivideBy(std::string str, char c);

} // namespace utils

#endif
