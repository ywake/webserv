#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include "result.hpp"
#include "thin_string.hpp"
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

std::vector<ThinString> Split(const ThinString &str, const std::string delim);
Result<long>            StrToLongDenyPadZero(const std::string &str);
bool                    IsRegularFile(const std::string &path);
bool                    EndsWith(const std::string &str, const std::string &suffix);
void                    Xpipe(int *fds);
void                    Xclose(int fd);
void                    Xdup2(int oldfd, int newfd);

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

	template <bool Cond, class T = void>
	struct enable_if {
	};

	template <class T>
	struct enable_if<true, T> {
		typedef T type;
	};

	template < typename T >
	std::string
	NumToStr(T number, typename enable_if<std::numeric_limits<T>::is_integer>::type * = 0)
	{
		std::ostringstream stream;
		stream << number;
		return stream.str();
	}
} // namespace utils

#endif
