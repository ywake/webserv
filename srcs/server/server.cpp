#include "server.hpp"
#include "listener.hpp"

namespace server
{
	Server::Server(const conf::ServerConfs &configs) : configs_(configs), io_monitor_(kEpollTimeout)
	{
		// lisnters 作成
		//  for (configs_)
	}

} // namespace server
