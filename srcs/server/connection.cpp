#include "connection.hpp"

namespace server
{
	const conf::VirtualServerConfs Connection::kEmptyConfs = conf::VirtualServerConfs();

	Connection::Connection() : Socket(), configs_(kEmptyConfs), client_() {}

	Connection::Connection(
		ManagedFd fd, const conf::VirtualServerConfs &configs, const SockAddrIn &client
	)
		: Socket(fd), configs_(configs), client_(client)
	{}

	Connection::Connection(const Connection &other)
		: Socket(other), configs_(other.configs_), client_(other.client_)
	{}

	bool Connection::operator<(const Connection &other)
	{
		return this->GetFd() < other.GetFd();
	}

	Connection::~Connection() {}

	event::Instructions Connection::Proceed()
	{
		return event::Instructions();
	}

} // namespace server
