#ifndef SOCKET_HPP
#define SOCKET_HPP
#include "result.hpp"
#include <map>
#include <utility>

class Socket
{
	typedef std::pair< std::string, std::string > HostPort;

  private:
	virtual Result< void > CreateSocket() = 0;

  protected:
	static std::map< HostPort, std::size_t > fd_count_;
	const char *host_;
	const char *port_;
	HostPort hostport_;
	int fd_;

  public:
	Socket(const char *host, const char *port) :
		host_(host), port_(port), hostport_(host, port), fd_(-1)
	{
		fd_count_[hostport_]++;
	};
	virtual Result< void > CreateSocketOnce()
	{
		if (fd_ != -1) {
			return Result< void >{};
		}
		return CreateSocket();
	};
	virtual ~Socket()
	{
		if (--fd_count_[hostport_] == 0 && fd_ != -1) {
			close(fd_);
		}
	};
};

#endif
