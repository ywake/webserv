#ifndef SERVER_TYPES_HPP
#define SERVER_TYPES_HPP

#include <list>
#include <netinet/in.h>
#include <set>
#include <vector>

#include "connection.hpp"
#include "listener.hpp"
#include "poll_instruction.hpp"

// TODO こいつincludeすると色んなとこ依存してよくないかも
namespace server
{
	typedef std::vector<Listener> Listeners;
	typedef std::set<Connection>  Connections;
} // namespace server

#endif
