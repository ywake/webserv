#ifndef SERVER_HPP
#define SERVER_HPP

#include <set>

#include "connection.hpp"
#include "event/event_pool.hpp"
#include "io_multiplexer.hpp"
#include "server_types.hpp"
#include "virtual_servers.hpp"
namespace server
{
	class Server
	{
	  private:
		Connections          connections_;
		conf::VirtualServers configs_;
		IoMultiplexer        io_monitor_;
		event::EventPool     event_pool_;
		Server();

	  public:
		Server(const conf::VirtualServers &configs);
		void Run();
	};
} // namespace server
#endif
