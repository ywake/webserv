#include "callback.hpp"

#include <iostream>

#include "debug.hpp"
#include "event.hpp"
#include "server.hpp"
#include <unistd.h>
#include <vector>

Event OnAccept(int fd, Server *s)
{
	log(fd, ": OnAccept()");
	int conn_fd = accept(fd, NULL, NULL);
	log(fd, ": After OnAccept()");
	return Event(conn_fd, s, ACCEPT);
}

Event OnServe(int fd, Server *s)
{
	log(s->listen_fd_, ": OnServe()");

	send(fd, s->port_.c_str(), s->port_.length() + 1, 0);
	// close(fd);
	return Event(fd, s, SERVE);
}

Event OnClose(int fd, Server *s)
{
	log(s->listen_fd_, ": OnClose()");
	close(fd);
	return Event(fd, s, CLOSE);
}
