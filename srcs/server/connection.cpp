#include "connection.hpp"

namespace server
{
	const conf::VirtualServerConfs Connection::kEmptyConfs = conf::VirtualServerConfs();

	Connection::Connection() : Socket(), configs_(kEmptyConfs), client_() {}

	Connection::Connection(
		int fd, const conf::VirtualServerConfs &configs, const SockAddrStorage &client
	)
		: Socket(fd), configs_(configs), client_(client)
	{}

	Connection::Connection(const Connection &other)
		: Socket(other), configs_(other.configs_), client_(other.client_)
	{}

	bool Connection::operator<(const Connection &other) const
	{
		return this->GetFd() < other.GetFd();
	}

	Connection::~Connection() {}

	event::Instructions Connection::Proceed(const event::Event &event)
	{
		if (event.fd == this->GetFd()) {
			return CommunicateWithClient(event.event_type);
		} else {
		}
		return event::Instructions();
	}

	event::Instructions Connection::CommunicateWithClient(uint32_t event_type)
	{
		event::Instructions insts;

		if (event_type & event::Event::kRead) {
			event::Instructions recv_insts = Recieve();
			insts.splice(insts.end(), recv_insts);
		}
		if (event_type & event::Event::kWrite) {
			event::Instructions send_insts = Send();
			insts.splice(insts.end(), send_insts);
		}
		return insts;
	}

	event::Instructions Connection::Recieve()
	{
		return event::Instructions();
	}

	event::Instructions Connection::Send()
	{
		return event::Instructions();
	}

} // namespace server
