#ifndef SERVER_TYPES_HPP
#define SERVER_TYPES_HPP

#include <set>

#include "connection.hpp"
#include "virtual_servers.hpp"

namespace server
{
	typedef std::set<Connection> Connections;
	typedef conf::VirtualServers Configs;
} // namespace server

#endif
