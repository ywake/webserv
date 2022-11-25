#include <cstdio>
#include <unistd.h>

namespace utils
{
	void Close(int fd)
	{
		if (close(fd) < 0) {
			perror("close: ");
		}
	}
} // namespace utils
