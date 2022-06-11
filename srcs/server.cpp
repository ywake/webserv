#include <cerrno>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server.hpp"

Server::Server(std::string port = "80")
	: port_(port), listen_fd_() {}

Server::Server(const Server &server) {}
Server &Server::operator=(const Server &server) {}
Server::~Server() {}

Result< void > Server::CreateListenSocket()
{
	t_addrinfo *lst;
	t_addrinfo hints = {
		.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV,
		.ai_socktype = SOCK_STREAM,
	};

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

Result< int > Server::TryBindSocket(t_addrinfo *lst)
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

// Result< void > Accept();
// Result< void > Serve();
// Result< void > Close();
