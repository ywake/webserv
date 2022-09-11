#include "event_result.hpp"

EventResult::EventResult() : fd_(-1), server_(), is_persistence_() {}

EventResult::EventResult(int fd, Server *server, State state, bool is_persistence)
	: fd_(fd), server_(server), state_(state), is_persistence_(is_persistence)
{}

EventResult::EventResult(const EventResult &copy)
	: fd_(copy.fd_),
	  server_(copy.server_),
	  state_(copy.state_),
	  is_persistence_(copy.is_persistence_)
{}

EventResult::~EventResult() {}
