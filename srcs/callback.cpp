#include "callback.hpp"

#include <iostream>

#include "debug.hpp"
#include "server.hpp"
#include <unistd.h>
#include <vector>

#include "event_pool.hpp"
#include "fd.hpp"
namespace Callback
{
	FdInfo Accept(int fd, Server *s)
	{
		log(s->listen_fd_, ": OnAccept()");
		int conn_fd = accept(fd, NULL, NULL);
		log(s->listen_fd_, ": After OnAccept()");
		return FdInfo(conn_fd, State::FdState(State::RECV, s));
	}

	FdInfo Serve(int fd, Server *s)
	{
		log(s->listen_fd_, ": OnServe()");

		send(fd, s->port_.c_str(), s->port_.length() + 1, 0);
		// close(fd);

		return FdInfo(fd, State::FdState(State::END, s));
	}
} // namespace Callback