#include "socket.hpp"

namespace server
{
	Socket::Socket() : managed_fd_() {}

	Socket::Socket(ManagedFd managed_fd) : managed_fd_(managed_fd) {}

	Socket::Socket(const Socket &other) : managed_fd_(other.managed_fd_) {}

	Socket::~Socket() {}

	int Socket::GetFd() const
	{
		return managed_fd_.GetFd();
	}

	Socket &Socket::operator=(const Socket &other)
	{
		if (this != &other) {
			managed_fd_ = other.managed_fd_;
		}
		return *this;
	}
} // namespace server
