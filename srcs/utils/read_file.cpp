#include "result.hpp"
#include <fcntl.h>
#include <fstream>
#include <string>
#include <unistd.h>

namespace utils
{

	Result<std::string> ReadFile(const std::string &file_path)
	{
		const int kReadFileBufferSize = 1024;

		int fd = open(file_path.c_str(), O_RDONLY);
		if (fd == -1) {
			return Error("open");
		}

		std::string file;
		char        buf[kReadFileBufferSize + 1];
		int         rs;
		while ((rs = read(fd, buf, kReadFileBufferSize)) > 0) {
			buf[rs] = 0;
			file += std::string(buf);
		}
		if (rs == -1) {
			return Error("read");
		}
		return file;
	}
} // namespace utils
