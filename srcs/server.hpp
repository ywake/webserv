#ifndef SERVER_HPP
#define SERVER_HPP

#include <set>

#include "connection.hpp"
#include "virtual_servers.hpp"

class Server
{
  private:
	std::set<Connection> conections_;
	conf::VirtualServers configs_;
	Server();

  public:
	Server(const conf::VirtualServers &configs);
	void Run();
};

#endif
