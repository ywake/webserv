#ifndef HANDLE_CONNECTION_HPP
#define HANDLE_CONNECTION_HPP

#include "i_event.hpp"
#include "poll.hpp"
#include "server_types.hpp"
namespace event
{
	class HandleConnection : public IEvent
	{
	  private:
		io_multiplexer::Poll &io_monitor_;
		server::Connections  &connections_;

	  private:
		HandleConnection();

	  public:
		HandleConnection(io_multiplexer::Poll &io_monitor, server::Connections &connections);
		void Run();
	};
} // namespace event
#endif
