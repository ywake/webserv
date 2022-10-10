#include "server.hpp"
#include "virtual_servers.hpp"

int main()
{
	// config_parse;
	server::Server server = server::Server(conf::VirtualServers());
	server.Run();
}
