#ifndef EVENT_HPP
#define EVENT_HPP

#include "callback.hpp"
#include "debug.hpp"
#include "event_result.hpp"
#include "server.hpp"

#include <vector>

class Event
{
  public:
	int fd_;
	Server *server_;
	Callback func_;
	Event()
		: fd_(-1), server_(NULL), func_(NULL){};
	Event(int fd, Server *server, Callback func)
		: fd_(fd), server_(server), func_(func){};
	Event(const Event &copy)
		: fd_(copy.fd_), server_(copy.server_), func_(copy.func_){};
	~Event(){};
	EventResult Run()
	{
		log(fd_, "event run");
		return func_(fd_, server_);
	}
};

#endif
