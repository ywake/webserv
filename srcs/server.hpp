#ifndef SERVER_HPP
#define SERVER_HPP

#include "server_socket.hpp"

class Server
{
  private:
	ServerSocket listen_sock_;
	Server();

  public:
	Server(ServerSocket &socket);
	// Server(const Server &server);
	// Server &operator=(const Server &server);
	~Server();
	Result< void > Run();
	int getFd(); //[TODO]　後で消す！
	// Result< void > Accept();
	// Result< void > Serve();
	// Result< void > Close();
};

#endif // SERVER_HPP
