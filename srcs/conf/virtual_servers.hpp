#ifndef VIRTUAL_SERVERS_HPP
#define VIRTUAL_SERVERS_HPP
#include <map>
#include <string>

#include "conf_types.hpp"
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
} // namespace conf
#endif
