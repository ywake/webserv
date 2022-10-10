#ifndef CLOSE_CONNECTION_HPP
#define CLOSE_CONNECTION_HPP

#include <set>

#include "connection.hpp"
#include "i_event.hpp"
#include "server_types.hpp"

namespace event
{
	class CloseConnection : public IEvent
	{
	  private:
		server::Connections &connections_;

	  private:
		CloseConnection();

	  public:
		CloseConnection(server::Connections &connections);
		void Run();
	};
} // namespace event
#endif
