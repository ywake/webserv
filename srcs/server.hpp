#ifndef SERVER_HPP
#define SERVER_HPP

#include <netdb.h>
#include <string>
#include <sys/socket.h>

#include "debug.hpp"

class Server
{
	typedef struct sockaddr Sockaddr;
	typedef struct sockaddr_storage SockaddrStorage;
	typedef struct addrinfo AddrInfo;

  private:
  public:
	std::string port_;
	int listen_fd_;
	Server(std::string port = "80")
		: port_(port)
	{
		log("server start", port);
		AddrInfo *lst;
		AddrInfo hints = {};

		hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;
		hints.ai_socktype = SOCK_STREAM;
		getaddrinfo(NULL, port_.c_str(), &hints, &lst);

		// try bind socket
		int optval = 1;
		for (; lst; lst = lst->ai_next) {
			listen_fd_ = socket(lst->ai_family, lst->ai_socktype, lst->ai_protocol);
			log("listen_fd", listen_fd_);
			setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
			if (bind(listen_fd_, lst->ai_addr, lst->ai_addrlen) == 0) {
				break;
			}
		}
		freeaddrinfo(lst);
		listen(listen_fd_, 1024);
	};
	~Server(){};
};

#endif
