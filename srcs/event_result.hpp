#ifndef EVENT_RESULT_HPP
#define EVENT_RESULT_HPP

#include "debug.hpp"
#include "server.hpp"
#include "state.hpp"
#include <vector>

class EventResult
{
  public:
	int fd_;
	Server *server_;
	State state_;
	EventResult();
	EventResult(int fd, Server *server, State state = ACCEPT);
	EventResult(const EventResult &copy);
	~EventResult();
};

#endif
