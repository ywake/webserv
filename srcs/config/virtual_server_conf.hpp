#ifndef VIRTUAL_SERVER_CONF_HPP
#define VIRTUAL_SERVER_CONF_HPP

#include <map>
#include <string>

#include "config_typedef.hpp"
#include "location_conf.hpp"

namespace conf
{
	class VirtualServerConf
	{
	  private:
		Port                         listen_port_;
		std::string                  server_name_;
		std::map<StatusCode, Path>   error_pages_;
		std::size_t                  client_max_body_size_;
		std::map<Path, LocationConf> location_conf_;

	  public:
		VirtualServerConf(
			Port                         listen_port          = "",
			std::string                  server_name          = "",
			std::map<StatusCode, Path>   error_pages          = std::map<StatusCode, Path>(),
			std::size_t                  client_max_body_size = 1UL << 20,
			std::map<Path, LocationConf> location_conf        = std::map<Path, LocationConf>()
		);
		~VirtualServerConf();
	};

	VirtualServerConf::VirtualServerConf(
		Port                         listen_port,
		std::string                  server_name,
		std::map<StatusCode, Path>   error_pages,
		std::size_t                  client_max_body_size,
		std::map<Path, LocationConf> location_conf
	)
	{}

	VirtualServerConf::~VirtualServerConf() {}
} // namespace conf

#endif
