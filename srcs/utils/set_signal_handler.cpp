#include <cerrno>
#include <csignal>
#include <cstring>

#include "result.hpp"

namespace utils
{
	Result<void> SetSignalHandler(int signum, sig_t handler, int sa_flags)
	{
		struct sigaction act;

		act.sa_handler = handler;
		act.sa_flags   = sa_flags;
		if (sigemptyset(&act.sa_mask) == -1) {
			return Error("sigemptyset: " + std::string(strerror(errno)));
		}
		if (sigaction(signum, &act, NULL) == -1) {
			return Error("sigemptyset: " + std::string(strerror(errno)));
		}
		return Result<void>();
	}

} // namespace utils
