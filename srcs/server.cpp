#include <cerrno>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server.hpp"

Server::Server(std::string port)
	: port_(port), listen_fd_() {}

Server::Server(const Server &server)
{
	(void)server;
}
Server &Server::operator=(const Server &server)
{
	(void)server;
	return *this;
}
Server::~Server() {}

Result< void > Server::CreateListenSocket()
{
	AddrInfo *lst;
	AddrInfo hints = {};

	hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;
	hints.ai_socktype = SOCK_STREAM;
	int err = getaddrinfo(NULL, port_.c_str(), &hints, &lst);
	if (err != 0) {
		return Result< void >(Error(gai_strerror(err)));
	}
	Result< int > res = TryBindSocket(lst);
	if (res.IsOk()) {
		listen_fd_ = Fd(res.Val());
	}
	freeaddrinfo(lst);
	return Result< void >(res.err);
}

Result< int > Server::TryBindSocket(AddrInfo *lst)
{
	int optval = 1;

	for (; lst; lst = lst->ai_next) {
		errno = 0;
		int sock_fd = socket(lst->ai_family, lst->ai_socktype, lst->ai_protocol);
		if (sock_fd < 0) {
			continue;
		}
		if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0) {
			return Result< int >(-1, Error(errno));
		}
		if (bind(sock_fd, lst->ai_addr, lst->ai_addrlen) == 0) {
			return Result< int >(sock_fd);
		}
		if (close(sock_fd) < 0) {
			return Result< int >(-1, Error(errno));
		}
	}
	return Result< int >(-1, Error("bind fail"));
}

Result< void > Server::Listen()
{
	if (listen_fd_.GetFd() != Fd::kNofd) {
		return Result< void >();
	}

	Result< void > res = CreateListenSocket();
	if (res.IsErr()) {
		return res;
	}

	if (listen(listen_fd_.GetFd(), kListenq) < 0) {
		return Result< void >(Error(errno));
	}
	return Result< void >();
}

Result< void > Server::Accept()
{
	int connfd = accept(listen_fd_.GetFd(), NULL, NULL);
	if (connfd < 0) {
		return Result< void >(Error(errno));
	}
	connected_socket_ = ConnectedSocket(connfd);
	return Result< void >();
}

Result< void > Server::Serve()
{
	Result< ssize_t > res_recv = connected_socket_.Recv();
	if (res_recv.IsErr()) {
		return Result< void >(Error(errno));
	}
	Result< ssize_t > res_send = connected_socket_.Send();
	if (res_recv.IsErr()) {
		return Result< void >(Error(errno));
	}
	connected_socket_.~ConnectedSocket();
	return Result< void >();
}
// Result< void > Close();
