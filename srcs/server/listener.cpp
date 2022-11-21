#include "listener.hpp"

#include <cerrno>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

namespace server
{
	typedef struct sockaddr SockAddr;

	/**
	 * @brief Construct a new Listener:: Listener object
	 *
	 * @param port
	 * @param configs
	 * @throw std::runtime_error
	 */
	Listener::Listener(const std::string &port, const conf::VirtualServerConfs &configs)
		: configs_(configs)
	{
		(void)port;
		// AddrInfo *lst;
		// AddrInfo  hints = {};

		// hints.ai_flags    = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;
		// hints.ai_socktype = SOCK_STREAM;
		// int err           = getaddrinfo(NULL, port.c_str(), &hints, &lst);
		// if (err != 0) {
		// 	// return Result<void>(Error(gai_strerror(err)));
		// }
		// Result<int> res = TryBindSocket(lst);
		// if (res.IsOk()) {
		// 	managed_fd_ = ManagedFd(res.Val());
		// }
		// freeaddrinfo(lst);
		// return Result<void>(Error(res.Err()));
	}

	Result<void> Listener::Listen() const
	{
		if (listen(managed_fd_.GetFd(), 1024) < 0) {
			return Error(std::string("listen: ") + strerror(errno));
		}
		return Result<void>();
	}

	Result<Connection> Listener::Accept() const
	{
		SockAddrStorage client;
		socklen_t       client_len = sizeof(client);

		int conn_fd = accept(managed_fd_.GetFd(), (SockAddr *)&client, &client_len);
		if (conn_fd < 0) {
			return Error("accept: " + std::string(strerror(errno)));
		}
		if (fcntl(conn_fd, F_SETFL, O_NONBLOCK) == -1) {
			utils::Close(conn_fd);
			return Error("fcntl: " + std::string(strerror(errno)));
		}
		return Connection(conn_fd, configs_, client);
	}

} // namespace server
