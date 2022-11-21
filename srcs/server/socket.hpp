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
		Socket(int fd);
		Socket(const Socket &other);
		int GetFd() const;
		virtual ~Socket();

		Socket &operator=(const Socket &other);
	};

} // namespace server

#endif
