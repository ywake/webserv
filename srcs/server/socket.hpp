#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netdb.h>

#include "managed_fd.hpp"

namespace server
{
	typedef struct sockaddr_storage SockAddrStorage;

	class Socket
	{
	  protected:
		ManagedFd       managed_fd_;
		SockAddrStorage addr_;

	  public:
		Socket();
		Socket(int fd, const SockAddrStorage &addr);
		Socket(const Socket &other);
		int GetFd() const;
		virtual ~Socket();

		Socket &operator=(const Socket &other);
	};

} // namespace server

#endif
