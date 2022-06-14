#include <vector>

#include "callback.hpp"
#include "event_pool.hpp"
#include "select.hpp"
#include "server.hpp"
#include "state.hpp"

void ServersInit(EventPool &pool, std::vector<Server> &servers)
{
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it) {
		Server *s = &(*it);
		pool.UpdateState(State::FdState(s->listen_fd_, State::LISTEN, s));
	}
}

#include <fcntl.h>

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
		Result<std::vector<int> > ret = pool.MonitorFds(&selector);
		if (ret.IsErr()) {
			log("monitor", ret.Err());
			continue;
		}
		pool.TriggerEvents(ret.Val());
	}
}
