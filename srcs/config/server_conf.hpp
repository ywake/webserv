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
	  public:
		typedef ConfElem<std::vector<Port> >            ListenPort;
		typedef ConfElem<std::vector<std::string> >     ServerName;
		typedef ConfElem<std::map<StatusCode, Path> >   ErrorPages;
		typedef ConfElem<std::size_t>                   ClientMaxBodySize;
		typedef ConfElem<std::map<Path, LocationConf> > LocationConfs;

	  private:
		ListenPort        listen_port_;
		ServerName        server_name_;
		ErrorPages        error_pages_;
		ClientMaxBodySize client_max_body_size_;
		LocationConfs     location_confs_;

	  public:
		VirtualServerConf(
			ListenPort        listen_port          = ListenPort(),
			ServerName        server_name          = ServerName(),
			ErrorPages        error_pages          = ErrorPages(),
			ClientMaxBodySize client_max_body_size = 1UL << 20,
			LocationConfs     location_conf        = LocationConfs()
		);
		~VirtualServerConf();

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

		bool operator==(const VirtualServerConf &rhs) const;
	};

	std::ostream &operator<<(std::ostream &os, const VirtualServerConf &conf);
} // namespace conf

#endif
