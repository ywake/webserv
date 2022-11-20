#include "server.hpp"
#include "debug.hpp"
#include "listener.hpp"

namespace server
{
	Server::Server(const conf::ServerConfs &configs) : configs_(configs), io_monitor_(kEpollTimeout)
	{
		typedef conf::ServerConfs::ConfsMap::const_iterator ConfsItr;
		for (ConfsItr it = configs_.begin(); it != configs_.end(); it++) {
			const std::string              &port     = it->first;
			const conf::VirtualServerConfs &conf     = it->second;
			Listener                        listener = Listener(port, conf);
			listeners_.push_back(listener);
		}
	}

	Result<void> Server::Listen()
	{
		for (Listeners::iterator it = listeners_.begin(); it != listeners_.end(); it++) {
			Result<void> listen_result = it->Listen();
			if (listen_result.IsErr()) {
				return Error(listen_result.Err());
			}
			uint32_t           event_type     = event::Event::kWrite | event::Event::kRead;
			event::Event       ev             = {it->GetFd(), &*it, event_type};
			event::Instruction instruction    = {event::Instruction::kRegister, ev};
			Result<void>       intruct_result = io_monitor_.Instruct(instruction);
			if (intruct_result.IsErr()) {
				Error(intruct_result.Err());
			}
		}
		return Result<void>();
	}

	void Server::Run()
	{
		while (true) {
			Result<event::Events> events = io_monitor_.Wait();
			if (events.IsErr()) {
				log(events.Err());
			}
			event::Instructions instructions            = RunEvents(events.Val());
			event::Instructions unregister_instructions = CloseFinishedConnections();
			instructions.splice(instructions.end(), unregister_instructions);
			io_multiplexer::ErrEvents err = io_monitor_.InstructMulti(instructions);
			if (!err.empty()) {
				log(err);
			}
		}
	}

	event::Instructions Server::RunEvents(const event::Events &events)
	{
		(void)events;
		return event::Instructions();
	}

	event::Instructions Server::CloseFinishedConnections()
	{
		return event::Instructions();
	}

} // namespace server
