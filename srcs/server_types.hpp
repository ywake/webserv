#ifndef SERVER_TYPES_HPP
#define SERVER_TYPES_HPP

#include <list>
#include <set>

#include "connection.hpp"
#include "poll_instruction.hpp"
#include "virtual_servers.hpp"

namespace server
{
	typedef std::set<Connection>                       Connections;
	typedef conf::VirtualServers                       Configs;
	typedef std::list<io_multiplexer::PollInstruction> PollInstructions;
} // namespace server

#endif
