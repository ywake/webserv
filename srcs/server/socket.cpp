#include "socket.hpp"

namespace server
{
	Socket::Socket() : managed_fd_(), addr_() {}

	Socket::Socket(int fd, const SockAddrStorage &addr) : managed_fd_(fd), addr_(addr) {}

	Socket::Socket(const Socket &other)
	{
		*this = other;
	}

	Socket::~Socket() {}

	int Socket::GetFd() const
	{
		return managed_fd_.GetFd();
	}

	Socket &Socket::operator=(const Socket &other)
	{
		if (this != &other) {
			managed_fd_ = other.managed_fd_;
			addr_       = other.addr_;
		}
		return *this;
	}
} // namespace server
