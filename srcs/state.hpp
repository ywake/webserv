#ifndef STATE_HPP
#define STATE_HPP

#include <utility>

#include "fd.hpp"
#include "server.hpp"

namespace State
{
	typedef enum {
		LISTEN = 0,
		RECV,
		SEND,
		END
	} State;
	typedef struct FdState {
		State state_;
		Server *server_;
		FdState(State state = END, Server *server = NULL)
			: state_(state), server_(server) {}
	} FdState;
	typedef struct FdInfo {
		Fd fd_;
		FdState state_;
		FdInfo(int fd, FdState state)
			: fd_(fd), state_(state) {}
	} FdInfo;
} // namespace State
#endif
