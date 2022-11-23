#include "connection.hpp"

namespace server
{
	const std::size_t              Connection::kMaxRecverBufSize = 10;
	const std::size_t              Connection::kMaxSenderBufSize = 10;
	const conf::VirtualServerConfs Connection::kEmptyConfs       = conf::VirtualServerConfs();

	Connection::Connection() : Socket(), configs_(kEmptyConfs), client_() {}

	Connection::Connection(
		int fd, const conf::VirtualServerConfs &configs, const SockAddrStorage &client
	)
		: Socket(fd), configs_(configs), client_(client), reciever_(fd)
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
		if (!reciever_.IsEof() && reciever_.size() < kMaxRecverBufSize) {
			Result<void> res = reciever_.Recv();
			if (res.IsErr()) {
				std::cerr << res.Err() << std::endl;
			}
		}
		event::Instructions insts = ParseRequest();
		return insts;
	}

	event::Instructions Connection::ParseRequest()
	{
		return event::Instructions();
	}

	event::Instructions Connection::Send()
	{
		return event::Instructions();
	}

} // namespace server
