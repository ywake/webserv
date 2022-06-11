#ifndef SERVER_HPP
#define SERVER_HPP

#include "fd.hpp"
#include "result.hpp"

class Server
{
	typedef struct sockaddr t_sa;
	typedef struct sockaddr_storage t_sa_storage;
	typedef struct addrinfo t_addrinfo;

  private:
	static const int kListenq = 1024;
	std::string port_;
	Fd listen_fd_;
	Result< void > CreateListenSocket();
	Result< int > TryBindSocket(t_addrinfo *lst);

  public:
	Server(std::string port = "80");
	Server(const Server &server);
	Server &operator=(const Server &server);
	~Server();
	Result< void > Listen();
	// Result< void > Accept();
	// Result< void > Serve();
	// Result< void > Close();
};

#endif // SERVER_HPP
