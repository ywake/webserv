#ifndef SERVER_CONF_HPP
#define SERVER_CONF_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "config_typedef.hpp"
#include "location_conf.hpp"
#include "thin_string.hpp"

namespace conf
{
	class VirtualServerConf
	{
	  private:
		std::vector<Port>            listen_port_;
		std::vector<std::string>     server_name_;
		std::map<StatusCode, Path>   error_pages_;
		std::size_t                  client_max_body_size_;
		std::map<Path, LocationConf> location_conf_;

	  public:
		VirtualServerConf(
			std::vector<Port>            listen_port          = std::vector<Port>(),
			std::vector<std::string>     server_name          = std::vector<std::string>(),
			std::map<StatusCode, Path>   error_pages          = std::map<StatusCode, Path>(),
			std::size_t                  client_max_body_size = 1UL << 20,
			std::map<Path, LocationConf> location_conf        = std::map<Path, LocationConf>()
		);
		~VirtualServerConf();

		void SetParams(const std::vector<ThinString> &params);
		void AddListenPort(const std::vector<ThinString> &tokens);
		void AddServerName(const std::vector<ThinString> &tokens);
		void AddErrorPage(const std::vector<ThinString> &tokens);
		void AddClientMaxBodySize(const std::vector<ThinString> &tokens);
		void AddLocation(const ThinString &location, const std::vector<ThinString> &params);

		std::vector<Port>            GetListenPort() const;
		std::vector<std::string>     GetServerName() const;
		std::map<StatusCode, Path>   GetErrorPages() const;
		std::size_t                  GetClientMaxBodySize() const;
		std::map<Path, LocationConf> GetLocationConfs() const;

		bool operator==(const VirtualServerConf &rhs) const;
	};

	std::ostream &operator<<(std::ostream &os, const VirtualServerConf &conf);
} // namespace conf

#endif
