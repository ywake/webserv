#include "connection.hpp"

namespace server
{
	const conf::VirtualServerConfs Connection::kEmptyConfs = conf::VirtualServerConfs();

	Connection::Connection() : managed_fd_(), configs_(kEmptyConfs), client_() {}

	Connection::Connection(
		ManagedFd fd, const conf::VirtualServerConfs &configs, const SockAddrIn &client
	)
		: managed_fd_(fd), configs_(configs), client_(client)
	{}

	Connection::Connection(const Connection &other)
		: managed_fd_(other.managed_fd_), configs_(other.configs_), client_(other.client_)
	{}

	Connection::~Connection() {}

	event::Instructions Connection::Proceed()
	{
		return event::Instructions();
	}

} // namespace server
