#include "server.hpp"
#include "virtual_servers.hpp"

int main()
{
	// config_parse;
	Server server = Server(conf::VirtualServers());
	server.Run();
}
