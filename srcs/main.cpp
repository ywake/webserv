#include <vector>

#include "callback.hpp"
#include "event_pool.hpp"
#include "select.hpp"
#include "server.hpp"

void ServersInit(EventPool &pool, std::vector<Server> &servers)
{
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it) {
		Server *s = &(*it);
		pool.UpdateEvent(Event(s->listen_fd_, s, OnAccept));
	}
}

#define SELECT
#ifdef SELECT
typedef Select Selector; //これインターフェースとやってること同じじゃね？
#endif

int main()
{
	EventPool pool;
	std::vector<Server> servers;

	servers.push_back(Server("8080"));
	servers.push_back(Server("8880"));
	ServersInit(pool, servers);
	while (true) {
		Selector selector;
		std::vector<int> ready;
		Result<void> ret = pool.MonitorFds(&selector, ready);
		if (ret.IsErr()) {
			log("monitor", ret.Err());
			continue;
		}
		pool.TriggerEvents(ready);
	}
}
