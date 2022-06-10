#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP
#include "socket.hpp"

class ServerSocket : public Socket
{
	typedef struct sockaddr t_sa;
	typedef struct sockaddr_storage t_sa_storage;
	typedef struct addrinfo t_addrinfo;

  private:
	static const int kListenq = 1024;
	Result< void > CreateSocket();

  public:
	ServerSocket(const char *host, const char *port);
	Result< int > TryBindSocket(t_addrinfo *lst);
	Result< void > Listen();
	Result< void > Listen();
	~ServerSocket();
};

#endif

// server.CreateSocket()
