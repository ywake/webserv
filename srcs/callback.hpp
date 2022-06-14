#ifndef CALLBACK_HPP
#define CALLBACK_HPP

#include <vector>

#include "server.hpp"
#include "state.hpp"

namespace Callback
{
	typedef State::FdInfo (*Callback)(int fd, Server *s);

	State::FdInfo Accept(int fd, Server *s);
	State::FdInfo Serve(int fd, Server *s);
} // namespace Callback
#endif
