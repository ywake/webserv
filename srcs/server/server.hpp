#ifndef SERVER_HPP
#define SERVER_HPP

#include <set>
#include <vector>

#include "connection.hpp"
#include "epoll.hpp"
#include "listener.hpp"
#include "server_confs.hpp"

namespace server
{
	class Server
	{
	  private:
		typedef std::vector<Listener> Listeners;
		typedef std::set<Connection>  Connections;

	  private:
		static const int      kEpollTimeout = 10; // TODO cppで初期化かも
		Connections           connections_;
		Listeners             listeners_;
		conf::ServerConfs     configs_;
		io_multiplexer::Epoll io_monitor_;
		Server();

	  public:
		Server(const conf::ServerConfs &configs);
		Result<void> Listen();
		void         Run();
	};
} // namespace server
#endif
