#include <map>
#include <vector>

#include "server.hpp"
#include <sys/socket.h>
#include <sys/types.h>

#include <iostream>
#include <unistd.h>

typedef struct sockaddr t_sa;
typedef struct sockaddr_storage t_sa_storage;
typedef struct addrinfo t_addrinfo;

void run_event_loop(Server &server)
{
	while (true) {
		server.Accept();
		server.Serve();
	}
}

int main(void)
{
	// t_conf confs = Conf(filepath);

	// Vector< Server > servers = init(confs); // socket, bind, listen

	Server server;
	Result< void > res = server.Listen();
	if (res.IsErr()) {
		std::cout << res.Err() << std::endl;
	}
	run_event_loop(server); // select, accept, recv, send
}
