#ifndef CALLBACK_HPP
#define CALLBACK_HPP

#include <vector>

#include "server.hpp"
#include "state.hpp"

namespace Callback
{
	typedef State::FdState (*Callback)(int fd, Server *s);

	State::FdState Accept(int fd, Server *s);
	State::FdState Serve(int fd, Server *s);
} // namespace Callback
#endif
