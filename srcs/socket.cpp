#include "socket.hpp"
#include <unistd.h>

std::map< Socket::HostPort, std::size_t > Socket::fd_count_ = std::map< Socket::HostPort, std::size_t >();

Socket::Socket() :
	host_(), port_(), hostport_(), fd_(kNofd) {}

Socket::Socket(char *host, char *port) :
	host_(host), port_(port), hostport_(host, port), fd_(kNofd)
{
	fd_count_[hostport_]++;
}

Socket::Socket(const Socket &sock) :
	host_(sock.host_), port_(sock.port_), hostport_(sock.hostport_), fd_(sock.fd_)
{
	fd_count_[hostport_]++;
}

Socket &Socket::operator=(const Socket &sock)
{
	if (this == &sock) {
		return *this;
	}
	host_ = sock.host_;
	port_ = sock.port_;
	hostport_ = sock.hostport_;
	fd_ = sock.fd_;
	fd_count_[hostport_]++;
	return *this;
}

Result< void > Socket::CreateSocketOnce()
{
	if (fd_ != kNofd) {
		return Result< void >();
	}
	return CreateSocket();
}

Socket::~Socket()
{
	if (--fd_count_[hostport_] == 0 && fd_ != kNofd) {
		close(fd_);
	}
}

int Socket::getFd()
{
	return fd_;
}
