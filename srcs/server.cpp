#include "server.hpp"

Server::Server() :
	listen_sock_() {}

Server::Server(ServerSocket &listen_sock) :
	listen_sock_(listen_sock) {}

// Server::Server(const Server &server) {}
// Server &Server::operator=(const Server &server) {}
Server::~Server() {}

Result< void > Server::Run()
{
	Result< void > res = listen_sock_.CreateSocketOnce();
	if (res.IsErr()) {
		return res;
	}
	return listen_sock_.Listen();
}

// Result< void > Accept();
// Result< void > Serve();
// Result< void > Close();

//[TODO]　後で消す！
int Server::getFd()
{
	return listen_sock_.getFd();
}
