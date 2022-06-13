#include <vector>

#include "callback.hpp"
#include "event_pool.hpp"
#include "server.hpp"

void ServersInit(EventPool &pool, std::vector<Server> &servers)
{
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it) {
		Server *s = &(*it);
		pool.UpdateEvent(Event(s->listen_fd_, s, OnAccept));
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

	while (true) {
		std::vector<int> ready;
		Result<void> ret = pool.MonitorFds(ready);
		if (ret.IsErr()) {
			log("monitor", ret.Err());
			continue;
		}
		pool.TriggerEvents(ready);
	}
}
