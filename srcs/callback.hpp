#ifndef CALLBACK_HPP
#define CALLBACK_HPP

#include <vector>

#include "server.hpp"
#include "state.hpp"

namespace Callback
{
	typedef struct FdInfo {
		Fd fd_;
		State::FdState state_;
		FdInfo(int fd, State::FdState state)
			: fd_(fd), state_(state) {}
	} FdInfo;
	typedef FdInfo (*Callback)(int fd, Server *s);

	FdInfo Accept(int fd, Server *s);
	FdInfo Serve(int fd, Server *s);
} // namespace Callback
#endif
