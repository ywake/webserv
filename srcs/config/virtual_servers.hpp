#ifndef VIRTUAL_SERVERS_HPP
#define VIRTUAL_SERVERS_HPP

#include <map>
#include <string>

#include "config_typedef.hpp"
#include "host_port.hpp"
#include "server_conf.hpp"

namespace conf
{
	class VirtualServers
	{
	  private:
		std::map<Port, Host>                       default_host_;
		std::map<http::abnf::HostPort, ServerConf> server_confs_;

	  public:
		VirtualServers();
		~VirtualServers();

		const ServerConf &GetDefaultServer(const Port &port);
		const ServerConf &GetConfig(const Port &port, const Host &host);
	};

	VirtualServers::VirtualServers(/* args */) {}

	VirtualServers::~VirtualServers() {}
} // namespace conf

#endif
