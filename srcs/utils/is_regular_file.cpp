#include <cerrno>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "result.hpp"

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
} // namespace utils
