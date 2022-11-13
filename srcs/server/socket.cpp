#include "socket.hpp"

namespace server
{
	Socket::Socket() : managed_fd_() {}

	Socket::Socket(const Socket &other) : managed_fd_(other.managed_fd_) {}

	Socket::~Socket() {}

	Socket &Socket::operator=(const Socket &other)
	{
		if (this != &other) {
			managed_fd_ = other.managed_fd_;
		}
		return *this;
	}
} // namespace server
