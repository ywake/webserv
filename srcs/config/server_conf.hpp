#ifndef SERVER_CONF_HPP
#define SERVER_CONF_HPP

#include <map>
#include <string>

#include "config_typedef.hpp"
#include "location_conf.hpp"

namespace conf
{
	class ServerConf
	{
	  private:
		Port                         listen_port_;
		std::string                  server_name_;
		std::map<StatusCode, Path>   error_pages_;
		std::size_t                  client_max_body_size_;
		std::map<Path, LocationConf> location_conf_;

	  public:
		ServerConf(/* args */);
		~ServerConf();
	};

	ServerConf::ServerConf(/* args */) {}

	ServerConf::~ServerConf() {}
} // namespace conf

#endif
