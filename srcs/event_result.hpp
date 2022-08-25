#ifndef EVENT_RESULT_HPP
#define EVENT_RESULT_HPP

#include "debug.hpp"
#include "server.hpp"
#include "state.hpp"

class EventResult
{
  public:
	int fd_;
	Server *server_;
	State state_;
	bool is_persistence_;
	EventResult();
	EventResult(int fd, Server *server, State state, bool is_persistence = false);
	EventResult(const EventResult &copy);
	~EventResult();
};

#endif
