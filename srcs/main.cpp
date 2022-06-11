#include <map>
#include <vector>

#include "server.hpp"
#include <sys/socket.h>
#include <sys/types.h>

#include <iostream>
#include <unistd.h>

void serve_content(int connfd)
{
	char buf[1024] = {};

	int ret = recv(connfd, buf, 1024, 0);
	send(connfd, buf, ret, 0);
}

typedef struct sockaddr t_sa;
typedef struct sockaddr_storage t_sa_storage;
typedef struct addrinfo t_addrinfo;

void run_event_loop(Server &server)
{
	int connfd;
	t_sa_storage client_addr;
	socklen_t client_len = sizeof(t_sa_storage);
	fd_set read_set;

	FD_ZERO(&read_set);
	FD_SET(server.getFd(), &read_set);
	while (true) {
		connfd = accept(server.getFd(), (t_sa *)&client_addr, &client_len);
		std::cout << server.getFd() << std::endl;
		serve_content(connfd);
		close(connfd);
	}
}

int main(void)
{
	// t_conf confs = Conf(filepath);

	// Vector< Server > servers = init(confs); // socket, bind, listen

	ServerSocket sock = ServerSocket(NULL, (char *)"80");
	Server server(sock);
	Result< void > res = server.Run();
	if (res.IsErr()) {
		std::cout << res.Err() << std::endl;
	}
	run_event_loop(server); // select, accept, recv, send
}
