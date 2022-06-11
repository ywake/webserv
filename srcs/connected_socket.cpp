#include "connected_socket.hpp"

ConnectedSocket::ConnectedSocket(int fd = kNofd)
	: Fd(fd) {}
ConnectedSocket::~ConnectedSocket() {}
ConnectedSocket::ConnectedSocket(ConnectedSocket const &other)
	: Fd(other) {}
ConnectedSocket &ConnectedSocket::operator=(ConnectedSocket const &other)
{
	if (this == &other) {
		return *this;
	}
	Fd::operator=(other);
	return *this;
}

Result< ssize_t > ConnectedSocket::Recv() {}
Result< ssize_t > ConnectedSocket::Send() {}