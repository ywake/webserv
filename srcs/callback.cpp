#include "callback.hpp"

#include <iostream>

#include "debug.hpp"
#include "event.hpp"
#include "server.hpp"
#include <unistd.h>
#include <vector>

std::vector<Event> OnAccept(int fd, Server *s)
{
	log(s->listen_fd_, ": OnAccept()");
	int conn_fd = accept(fd, NULL, NULL);

	std::vector<Event> events;
	events.push_back(Event(fd, s, OnAccept));
	events.push_back(Event(conn_fd, s, OnServe));
	return events;
}

std::vector<Event> OnServe(int fd, Server *s)
{
	log(s->listen_fd_, ": OnServe()");

	send(fd, s->port_.c_str(), s->port_.length() + 1, 0);
	close(fd);

	std::vector<Event> events;
	return events;
}
