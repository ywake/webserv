#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <string>

#include "config/virtual_server_confs.hpp"
#include "connection.hpp"
#include "managed_fd.hpp"
#include "result.hpp"
#include "socket.hpp"

#include <netdb.h>
#include <sys/socket.h>

namespace server
{
	class Listener : public Socket
	{
	  private:
		typedef struct addrinfo AddrInfo;

	  private:
		const conf::VirtualServerConfs &configs_;

	  public:
		Listener(const std::string &port, const conf::VirtualServerConfs &configs);
		Result<void>         Listen() const;
		Result<Connection *> Accept() const;

	  private:
		Result<int> TryBindSocket(AddrInfo *lst);
	};
} // namespace server

#endif
