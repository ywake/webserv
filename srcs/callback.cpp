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
	State::FdState Accept(int fd, Server *s)
	{
		log(s->listen_fd_, ": OnAccept()");
		int conn_fd = accept(fd, NULL, NULL);
		log(s->listen_fd_, ": After OnAccept()");
		return State::FdState(conn_fd, State::RECV, s);
	}

	State::FdState Serve(int fd, Server *s)
	{
		log(s->listen_fd_, ": OnServe()");

		send(fd, s->port_.c_str(), s->port_.length() + 1, 0);
		// close(fd);

		return State::FdState(fd, State::END, s);
	}
} // namespace Callback