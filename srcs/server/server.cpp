#include "server.hpp"
#include "debug.hpp"
#include "listener.hpp"

namespace server
{
	using namespace event;

	const int Server::kEpollTimeout = 5000;

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
				return listen_result.Err();
			}
			Instruction instruction = Instruction(
				Instruction::kRegister, it->GetFd(), Event::kWrite | Event::kRead, &*it
			);
			Result<void> intruct_result = io_monitor_.Instruct(instruction);
			if (intruct_result.IsErr()) {
				return intruct_result.Err();
			}
		}
		return Result<void>();
	}

	void Server::Run()
	{
		while (true) {
			Result<Events> events = io_monitor_.Wait();
			if (events.IsErr()) {
				log(events.Err());
			}
			Instructions instructions            = RunEvents(events.Val());
			Instructions unregister_instructions = CloseFinishedConnections();
			instructions.splice(instructions.end(), unregister_instructions);
			io_multiplexer::ErrEvents err = io_monitor_.InstructMulti(instructions);
			if (!err.empty()) {
				log(err);
			}
		}
	}

	Instructions Server::RunEvents(const Events &events)
	{
		Instructions insts;

		for (Events::const_iterator it = events.begin(); it != events.end(); ++it) {
			Event        event = *it;
			Socket      *sock  = static_cast<Socket *>(event.data);
			Instructions new_instructions;
			if (dynamic_cast<Connection *>(sock)) {
				Connection *connection = dynamic_cast<Connection *>(sock);
				new_instructions       = connection->Proceed(event);
			} else if (dynamic_cast<Listener *>(sock)) {
				new_instructions = Accept(dynamic_cast<Listener *>(sock));
			} else {
				std::cerr << "unknown error" << std::endl;
			}
			insts.splice(insts.end(), new_instructions);
		}
		return insts;
	}

	Instructions Server::Accept(const Listener *listener)
	{
		typedef std::pair<server::Server::Connections::iterator, bool> Position;

		Instructions insts;

		Result<Connection> res = listener->Accept();
		if (res.IsErr()) {
			std::cerr << res.Err() << std::endl;
			return insts;
		}
		Position    pos      = connections_.insert(res.Val());
		Connection *conn_ptr = const_cast<Connection *>(&*pos.first);
		Instruction instruction =
			Instruction(Instruction::kRegister, conn_ptr->GetFd(), Event::kRead, conn_ptr);
		insts.push_back(instruction);
		return insts;
	}

	Instructions Server::CloseFinishedConnections()
	{
		std::vector<Connection> finished;
		Instructions            insts;
		for (Connections::iterator it = connections_.begin(); it != connections_.end(); ++it) {
			Connection &con = const_cast<Connection &>(*it);
			if (con.IsFinished()) {
				log("finish fd", con.GetFd());
				Instructions i = con.Disconnect();
				insts.splice(insts.end(), i);
				finished.push_back(con.GetFd());
			}
		}
		for (std::vector<Connection>::iterator it = finished.begin(); it != finished.end(); it++) {
			connections_.erase(*it);
		}
		return insts;
	}

} // namespace server
