#include "server.hpp"
#include "close_connection.hpp"
#include "handle_connection.hpp"

namespace server
{
	Server::Server() {}

	Server::Server(const conf::VirtualServers &configs) : configs_(configs)
	{
		// TODO listenerの登録
		// TODO listener:configのmapにパース
	}

	void Server::Run()
	{
		while (true) {
			event::HandleConnection connection_event(io_monitor_, connections_);
			event::CloseConnection  close_event(connections_);
			event_pool_.AddEvent(&connection_event);
			event_pool_.AddEvent(&close_event);
			event_pool_.Run();
		}
	}
} // namespace server
