#ifndef CONNECTED_SOCKET_HPP
#define CONNECTED_SOCKET_HPP

#include "fd.hpp"
#include "result.hpp"
#include <unistd.h>

class ConnectedSocket : public Fd
{
  private:
	char buf_[1024];
	ssize_t read_bytes_;

  public:
	ConnectedSocket(int fd = kNofd);
	~ConnectedSocket();
	ConnectedSocket(ConnectedSocket const &other);
	ConnectedSocket &operator=(ConnectedSocket const &other);
	Result< ssize_t > Recv();
	Result< ssize_t > Send();
};
#endif
