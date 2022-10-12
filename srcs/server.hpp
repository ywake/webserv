#ifndef SERVER_HPP
#define SERVER_HPP

#include <set>

#include "connection.hpp"
#include "event/event_pool.hpp"
#include "poll.hpp"
#include "server_confs.hpp"
#include "server_types.hpp"
namespace server
{
	class Server
	{
	  private:
		Connections          connections_;
		Listeners            listeners_;
		conf::ServerConfs    configs_;
		io_multiplexer::Poll io_monitor_;
		event::EventPool     event_pool_;
		Server();

	  public:
		Server(const conf::ServerConfs &configs);
		int  Listen();
		void Run();
	};
} // namespace server
#endif
