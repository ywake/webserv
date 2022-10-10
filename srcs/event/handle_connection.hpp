#ifndef HANDLE_CONNECTION_EVENT_HPP
#define HANDLE_CONNECTION_EVENT_HPP

#include "i_event.hpp"
#include "io_multiplexer.hpp"
#include "server_types.hpp"
namespace event
{
	class HandleConnection : public IEvent
	{
	  private:
		IoMultiplexer       &io_monitor_;
		server::Connections &connections_;

	  private:
		HandleConnection();

	  public:
		HandleConnection(IoMultiplexer &io_monitor, server::Connections &connections);
		void Run();
	};
} // namespace event
#endif
