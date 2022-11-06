#ifndef SERVER_CONF_HPP
#define SERVER_CONF_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "config_typedef.hpp"
#include "location_conf.hpp"
#include "result.hpp"
#include "thin_string.hpp"

namespace conf
{
	class ServerConf
	{
	  private:
		static const std::size_t kMaxClientBodySize = 1 << 30;

	  public:
		typedef std::vector<Port>            ListenPort;
		typedef std::vector<std::string>     ServerName;
		typedef std::map<StatusCode, Path>   ErrorPages;
		typedef Emptiable<std::size_t>       ClientMaxBodySize;
		typedef std::map<Path, LocationConf> LocationConfs;

	  private:
		ListenPort        listen_port_;
		ServerName        server_name_;
		ErrorPages        error_pages_;
		ClientMaxBodySize client_max_body_size_;
		LocationConfs     location_confs_;

	  public:
		ServerConf(
			ListenPort        listen_port          = ListenPort(),
			ServerName        server_name          = ServerName(),
			ErrorPages        error_pages          = ErrorPages(),
			ClientMaxBodySize client_max_body_size = ClientMaxBodySize(),
			LocationConfs     location_conf        = LocationConfs()
		);
		~ServerConf();

		void SetParams(const std::vector<ThinString> &params);
		void AddListenPort(const std::vector<ThinString> &tokens);
		void AddServerName(const std::vector<ThinString> &tokens);
		void AddErrorPages(const std::vector<ThinString> &tokens);
		void AddClientMaxBodySize(const std::vector<ThinString> &tokens);
		void AddLocation(const ThinString &location, const std::vector<ThinString> &params);

		const ListenPort        &GetListenPort() const;
		const ServerName        &GetServerName() const;
		const ErrorPages        &GetErrorPages() const;
		const ClientMaxBodySize &GetClientMaxBodySize() const;
		const LocationConfs     &GetLocationConfs() const;

		bool operator==(const ServerConf &rhs) const;
		bool operator!=(const ServerConf &rhs) const;
	};

	std::ostream &operator<<(std::ostream &os, const ServerConf &conf);
} // namespace conf

#endif
