#include "listener.hpp"

#include <cerrno>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

#include "listen_exception.hpp"
#include "webserv_utils.hpp"

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
		AddrInfo *lst;
		AddrInfo  hints = {};

		hints.ai_flags    = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;
		hints.ai_socktype = SOCK_STREAM;
		int err           = getaddrinfo(NULL, port.c_str(), &hints, &lst);
		if (err != 0) {
			throw ListenException("getaddrinfo failure");
		}
		Result<int> sock = TryBindSocket(lst);
		freeaddrinfo(lst);
		if (sock.IsErr()) {
			throw ListenException(sock.ErrMsg());
		}
		managed_fd_ = sock.Val();
	}

	Result<int> Listener::TryBindSocket(AddrInfo *lst)
	{
		int optval = 1;

		for (; lst; lst = lst->ai_next) {
			errno       = 0;
			int sock_fd = socket(lst->ai_family, lst->ai_socktype, lst->ai_protocol);
			if (sock_fd < 0) {
				continue;
			}
			if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0) {
				utils::Close(sock_fd);
				return Error(std::string("setsockopt: ") + strerror(errno));
			}
			if (bind(sock_fd, lst->ai_addr, lst->ai_addrlen) == 0) {
				return sock_fd;
			}
			utils::Close(sock_fd);
		}
		return Error(std::string("listener: ") + strerror(errno));
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
