#include <cerrno>
#include <cstdio>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <unistd.h>

#include "result.hpp"

namespace utils
{

	Result<std::string> ReadFile(const std::string &file_path)
	{
		const int kReadFileBufferSize = 1024;

		int fd = open(file_path.c_str(), O_RDONLY);
		if (fd == -1) {
			return Error(std::string("open: ") + strerror(errno));
		}

		std::string file;
		char        buf[kReadFileBufferSize + 1];
		int         read_count;
		while ((read_count = read(fd, buf, kReadFileBufferSize)) > 0) {
			file += std::string(buf, read_count);
		}
		if (read_count == -1) {
			return Error("read");
		}
		return file;
	}
} // namespace utils
