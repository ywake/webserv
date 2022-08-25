#include "callback.hpp"

#include <iostream>

#include "debug.hpp"
#include "server.hpp"
#include <unistd.h>

EventResult OnAccept(int fd, Server *s)
{
	log(fd, ": OnAccept()");
	int conn_fd = accept(fd, NULL, NULL);
	log(fd, ": After OnAccept()");
	return EventResult(conn_fd, s, ACCEPT, true);
}

EventResult OnServe(int fd, Server *s)
{
	log(s->listen_fd_, ": OnServe()");

	send(fd, s->port_.c_str(), s->port_.length() + 1, 0);
	// close(fd);
	return EventResult(fd, s, SERVE);
}

EventResult OnClose(int fd, Server *s)
{
	log(s->listen_fd_, ": OnClose()");
	close(fd);
	return EventResult(fd, s, CLOSE);
}
