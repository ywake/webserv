#include "result.hpp"
#include "webserv_utils.hpp"
#include <cerrno>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace utils
{
	Result<bool> IsRegularFile(int fd)
	{
		struct stat st;

		if (fstat(fd, &st) < 0) {
			return Error(std::string("fstat: ") + strerror(errno));
		}
		return S_ISREG(st.st_mode);
	}

	Result<bool> IsRegularFile(const std::string &path)
	{
		struct stat st;

		if (stat(path.c_str(), &st) != 0) {
			return Error(std::string("stat: ") + strerror(errno));
		}
		return S_ISREG(st.st_mode);
	}
} // namespace utils
