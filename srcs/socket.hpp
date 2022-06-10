#ifndef SOCKET_HPP
#define SOCKET_HPP
#include "result.hpp"
#include <map>
#include <utility>

class Socket
{
  public:
	typedef std::pair< char *, char * > HostPort;

  private:
	virtual Result< void > CreateSocket() = 0;

  protected:
	static std::map< HostPort, std::size_t > fd_count_;
	static const int kNofd = -2;
	char *host_;
	char *port_;
	HostPort hostport_;
	int fd_;

  public:
	Socket();
	Socket(char *host, char *port);
	Socket(const Socket &sock);
	Socket &operator=(const Socket &sock);
	Result< void > CreateSocketOnce();
	virtual ~Socket();

	// tmp
	int getFd();
};

#endif
