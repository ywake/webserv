#ifndef EVENT_HPP
#define EVENT_HPP

#include "debug.hpp"
#include "server.hpp"
#include "state.hpp"
#include <vector>

class Event
{
  public:
	int fd_;
	Server *server_;
	State state_;
	Event();
	Event(int fd, Server *server, State state = ACCEPT);
	Event(const Event &copy);
	~Event();
};

#endif
