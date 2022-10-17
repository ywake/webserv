#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "fd.hpp"
#include

namespace socket
{
	typedef struct sockaddr_in SockAddrIn;

	class Socket : public Fd
	{
	  private:
		SockAddrIn address_;

	  public:
		virtual ~Socket();
	};
} // namespace socket

#endif
