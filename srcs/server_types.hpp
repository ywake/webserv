#ifndef SERVER_TYPES_HPP
#define SERVER_TYPES_HPP

#include <list>
#include <set>
#include <vector>

#include "connection.hpp"
#include "listener.hpp"
#include "poll_instruction.hpp"
namespace server
{
	typedef std::vector<Listener>                      Listeners;
	typedef std::set<Connection>                       Connections;
	typedef std::list<io_multiplexer::PollInstruction> PollInstructions;
} // namespace server

#endif
