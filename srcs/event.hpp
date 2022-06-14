#ifndef EVENT_HPP
#define EVENT_HPP

#include "debug.hpp"
#include "server.hpp"
#include <vector>

class Event
{
	typedef std::vector<Event> CallbackReturn;
	typedef CallbackReturn (*Callback)(int fd, Server *s);

  public:
	typedef enum {
		RUNNING = 0,
		STOPPED
	} State;

  public:
	int fd_;
	Server *server_;
	Callback func_;
	State state_;
	Event()
		: fd_(-1), server_(NULL), func_(NULL){};
	Event(int fd, Server *server, Callback func, State state = RUNNING)
		: fd_(fd), server_(server), func_(func), state_(state){};
	Event(const Event &copy)
		: fd_(copy.fd_), server_(copy.server_), func_(copy.func_), state_(copy.state_){};
	~Event(){};
	std::vector<Event> Run()
	{
		log(fd_, "event run");
		return func_(fd_, server_);
	}
};

#endif
