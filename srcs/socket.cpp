#include "socket.hpp"

/*
 * Constructor/Destructor
 */
Socket::Socket() {}

Socket::Socket(char *host, char *port) :
	Fd(), host_(host), port_(port) {}

Socket::Socket(Socket const &copy) :
	Fd(copy), host_(copy.host_), port_(copy.port_) {}

Socket::~Socket() {}

/*
 * Operators
 */
Socket &Socket::operator=(Socket const &other)
{
	if (this == &other) {
		return *this;
	}
	Fd::operator=(other);
	host_ = other.host_;
	port_ = other.port_;
	return *this;
}

/*
 * Getter/Setter
 */

/*
 * Methods
 */
