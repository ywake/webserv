#ifndef STATE_HPP
#define STATE_HPP

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
		Fd fd_;
		State state_;
		Server *server_;
		FdState(int fd = -1, State state = END, Server *server = NULL)
			: fd_(fd), state_(state), server_(server) {}
	} FdState;
} // namespace State
#endif
