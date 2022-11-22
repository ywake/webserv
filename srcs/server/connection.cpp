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

	event::Instructions Connection::Recieve()
	{
		return event::Instructions();
	}

	event::Instructions Connection::Send()
	{
		return event::Instructions();
	}

} // namespace server
