#include "server_socket.hpp"
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

ServerSocket::ServerSocket(char *host, char *port) :
	Socket(host, port) {}

ServerSocket::~ServerSocket() {}

//[TODO] いっぱいlisten
Result< void > ServerSocket::Listen()
{
	Result< void > res = CreateSocketOnce();
	if (res.IsErr()) {
		return res;
	}
	if (listen(fd_, kListenq) < 0) {
		return Result< void >{Error(errno)};
	}
	return res;
}

Result< void > ServerSocket::CreateSocket()
{
	t_addrinfo *lst;
	t_addrinfo hints = {
		.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV,
		.ai_socktype = SOCK_STREAM,
	};

	int err = getaddrinfo(host_, port_, &hints, &lst);
	if (err != 0) {
		return Result< void >{Error(gai_strerror(err))};
	}
	Result< int > res = TryBindSocket(lst);
	fd_ = res.Val();
	freeaddrinfo(lst);
	return Result< void >{res.err};
}

Result< int > ServerSocket::TryBindSocket(t_addrinfo *lst)
{
	int optval = 1;

	for (; lst; lst = lst->ai_next) {
		int sock_fd = socket(lst->ai_family, lst->ai_socktype, lst->ai_protocol);
		if (sock_fd < 0) {
			continue;
		}
		if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0) {
			return Result< int >{-1, Error(errno)};
		}
		if (bind(sock_fd, lst->ai_addr, lst->ai_addrlen) == 0) {
			return Result< int >{sock_fd};
		}
		if (close(sock_fd) < 0) {
			return Result< int >{-1, Error(errno)};
		}
	}
	return Result< int >{-1, Error("bind fail")};
}