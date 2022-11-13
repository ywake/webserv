#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "managed_fd.hpp"

namespace server
{

	class Socket
	{
	  protected:
		ManagedFd managed_fd_;

	  public:
		Socket();
		Socket(const Socket &other);
		~Socket();

		Socket &operator=(const Socket &other);
	};

} // namespace server

#endif
