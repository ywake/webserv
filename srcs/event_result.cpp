#include "event_result.hpp"

EventResult::EventResult()
	: fd_(-1), server_() {}

EventResult::EventResult(int fd, Server *server, State state)
	: fd_(fd), server_(server), state_(state) {}

EventResult::EventResult(const EventResult &copy)
	: fd_(copy.fd_), server_(copy.server_), state_(copy.state_) {}

EventResult::~EventResult() {}
