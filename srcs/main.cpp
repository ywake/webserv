#include <vector>

#include "callback.hpp"
#include "event_pool.hpp"
#include "server.hpp"

void ServersInit(EventPool &pool, std::vector<Server> &servers)
{
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it) {
		Server *s = &(*it);
		pool.AddEvent(Event(s->listen_fd_, s, OnAccept));
	}
}
#include <fcntl.h>
int main()
{
	EventPool pool;

	std::vector<Server> servers;
	servers.push_back(Server("8080"));
	servers.push_back(Server("8880"));

	ServersInit(pool, servers);
	SelectFds set;
	while (true) {
		pool.CollectFds(&set);

		int ret = select(set.nfds, &set.read_set, NULL, NULL, NULL);
		log("select", ret);
		if (ret <= 0) {
			continue;
		}
		pool.EventsTrigger(set);
	}
}
