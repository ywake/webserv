#ifndef SERVER_HPP
#define SERVER_HPP

#include "connected_socket.hpp"
#include "fd.hpp"
#include "result.hpp"

class Server
{
	typedef struct sockaddr Sockaddr;
	typedef struct sockaddr_storage SockaddrStorage;
	typedef struct addrinfo AddrInfo;

  private:
	static const int kListenq = 1024;
	std::string port_;
	Fd listen_fd_;
	ConnectedSocket connected_socket_;

	Result< void > CreateListenSocket();
	Result< int > TryBindSocket(AddrInfo *lst);

  public:
	Server(std::string port = "80");
	Server(const Server &server);
	Server &operator=(const Server &server);
	~Server();

	Result< void > Listen();
	Result< void > Accept();
	// Result< void > Serve();
	// Result< void > Close();
};

#endif // SERVER_HPP
