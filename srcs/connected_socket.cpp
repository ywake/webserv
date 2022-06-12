#include "connected_socket.hpp"

#include <cerrno>
#include <sys/socket.h>
#include <unistd.h>

ConnectedSocket::ConnectedSocket(int fd)
	: Fd(fd), buf_(), read_bytes_(0) {}
ConnectedSocket::ConnectedSocket(ConnectedSocket const &other)
	: Fd(other), read_bytes_(other.read_bytes_)
{
	std::memcpy(buf_, other.buf_, read_bytes_);
}
ConnectedSocket::~ConnectedSocket() {}
ConnectedSocket &ConnectedSocket::operator=(ConnectedSocket const &other)
{
	if (this == &other) {
		return *this;
	}
	Fd::operator=(other);
	return *this;
}

Result< ssize_t > ConnectedSocket::Recv()
{
	read_bytes_ = recv(fd_, buf_, 1024, 0);
	if (read_bytes_ < 0) {
		return Result< ssize_t >(Error(errno));
	}
	return Result< ssize_t >(read_bytes_);
}

Result< ssize_t > ConnectedSocket::Send()
{
	ssize_t res = send(fd_, buf_, read_bytes_, 0);
	if (res < 0) {
		return Result< ssize_t >(Error(errno));
	}
	return Result< ssize_t >(res);
}
