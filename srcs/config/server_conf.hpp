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
		typedef std::vector<Port>          ListenPort;
		typedef std::vector<std::string>   ServerName;
		typedef std::map<StatusCode, Path> ErrorPages;
		typedef Emptiable<std::size_t>     ClientMaxBodySize;
		typedef std::vector<LocationConf>  LocationConfs;
		typedef Emptiable<Path>            Root;

	  private:
		ListenPort        listen_port_;
		ServerName        server_name_;
		ErrorPages        error_pages_;
		ClientMaxBodySize client_max_body_size_;
		LocationConfs     location_confs_;
		Root              root_;

	  private:
		static const ListenPort        kDefaultListenPort;
		static const ServerName        kDefaultServerName;
		static const ErrorPages        kDefaultErrorPages;
		static const ClientMaxBodySize kDefaultClientMaxBodySize;
		static const LocationConfs     kDefaultLocationConfs;
		static const LocationConf      kDefaultLocationConf;

	  public:
		ServerConf(
			Root              root                 = Root(),
			ListenPort        listen_port          = ListenPort(),
			ServerName        server_name          = ServerName(),
			ErrorPages        error_pages          = ErrorPages(),
			ClientMaxBodySize client_max_body_size = ClientMaxBodySize(),
			LocationConfs     location_conf        = LocationConfs()
		);
		~ServerConf();

		// Setters
		void SetParams(const std::vector<ThinString> &params);
		void AddListenPort(const std::vector<ThinString> &tokens);
		void AddServerName(const std::vector<ThinString> &tokens);
		void AddErrorPages(const std::vector<ThinString> &tokens);
		void AddClientMaxBodySize(const std::vector<ThinString> &tokens);
		void AddLocation(
			const ThinString              &location,
			const Path                    &default_root,
			const std::vector<ThinString> &params
		);
		void AddRoot(const std::vector<ThinString> &tokens);

		// Getters
		const ListenPort    &GetListenPort() const;
		const ServerName    &GetServerName() const;
		const ErrorPages    &GetErrorPages() const;
		const std::size_t   &GetClientMaxBodySize() const;
		const LocationConfs &GetLocationConfs() const;
		const Path          &GetRoot(Path uri_path) const;
		const Path          &GetRowRoot() const;

		// Methods
		const LocationConf &FindMatchingLocationConf(Path uri_path) const;

		// Operators
		bool operator==(const ServerConf &rhs) const;
		bool operator!=(const ServerConf &rhs) const;
	};

	std::ostream &operator<<(std::ostream &os, const ServerConf &conf);
} // namespace conf

#endif
