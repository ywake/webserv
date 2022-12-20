#include <cerrno>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>

#include "result.hpp"

namespace utils
{
	Result<std::string> CreateCurrentTimeStamp()
	{
		struct timespec ts;
		struct tm       tm;
		char            buf[1000];

		if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
			return Error("clock get: " + std::string(strerror(errno)));
		}
		if (gmtime_r(&ts.tv_sec, &tm) == NULL) {
			return Error("localtime_r: " + std::string(strerror(errno)));
		}
		sprintf(
			buf,
			"%d-%02d-%02d_%02d:%02d:%02d.%09ld",
			tm.tm_year + 1900,
			tm.tm_mon + 1,
			tm.tm_mday,
			tm.tm_hour,
			tm.tm_min,
			tm.tm_sec,
			ts.tv_nsec
		);
		return std::string(buf);
	}
} // namespace utils
