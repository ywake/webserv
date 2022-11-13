#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <string>

#include "config/virtual_server_confs.hpp"
#include "connection.hpp"
#include "managed_fd.hpp"
#include "result.hpp"

#include <netdb.h>
#include <sys/socket.h>

namespace server
{
	typedef struct addrinfo AddrInfo;

	class Listener
	{
	  private:
		ManagedFd                       managed_fd_;
		const conf::VirtualServerConfs &configs_;

	  public:
		Listener(const std::string &port, const conf::VirtualServerConfs &configs);
		Result<void>       Listen();
		Result<Connection> Accept();

	  private:
		Result<int> TryBindSocket(AddrInfo *lst);
	};
} // namespace server

#endif
