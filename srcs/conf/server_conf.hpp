#ifndef SERVER_CONF_HPP
#define SERVER_CONF_HPP

#include <map>

#include "conf_types.hpp"
#include "location_conf.hpp"
#include "status_code.hpp"

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
} // namespace conf
#endif
