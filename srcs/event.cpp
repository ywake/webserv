#include "event.hpp"

Event::Event()
	: fd_(-1), server_(){}

Event::Event(int fd, Server *server, State state)
	: fd_(fd), server_(server), state_(state){}

Event::Event(const Event &copy)
	: fd_(copy.fd_), server_(copy.server_), state_(copy.state_){}

Event::~Event(){}
