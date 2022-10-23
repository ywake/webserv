#include "server_conf.hpp"

#include "config_exceptions.hpp"
#include "webserv_utils.hpp"

namespace conf
{
	VirtualServerConf::VirtualServerConf(
		Port                         listen_port,
		std::string                  server_name,
		std::map<StatusCode, Path>   error_pages,
		std::size_t                  client_max_body_size,
		std::map<Path, LocationConf> location_conf
	)
		: listen_port_(listen_port),
		  server_name_(server_name),
		  error_pages_(error_pages),
		  client_max_body_size_(client_max_body_size),
		  location_conf_(location_conf)
	{}

	VirtualServerConf::~VirtualServerConf() {}

	void VirtualServerConf::SetParams(const std::vector<ThinString> &params)
	{
		for (std::vector<ThinString>::const_iterator it = params.begin(); it != params.end();
			 ++it) {

			std::vector<ThinString> split = Split(*it, " ");
			if (split.size() < 2) {
				throw ConfigException("Invalid config");
			}
			if (split[0] == "listen") {
				if (split.size() != 2) {
					throw ConfigException("Invalid listen");
				}
				listen_port_ = split[1].ToString();
			} else if (split[0] == "server_name") {
				if (split.size() != 2) {
					throw ConfigException("Invalid server_name");
				}
				server_name_ = split[1].ToString();
			} else if (split[0] == "error_page") {
				if (split.size() != 3) {
					throw ConfigException("Invalid error_page");
				}
				error_pages_[split[1].ToString()] = split[2].ToString();
			} else if (split[0] == "client_max_body_size") {
				if (split.size() != 2) {
					throw ConfigException("Invalid client_max_body_size");
				}
				client_max_body_size_ = std::stoul(split[1].ToString());
			} else {
				throw ConfigException("Invalid config");
			}
		}
	}

	void VirtualServerConf::AddLocation(
		const ThinString &location, const std::vector<ThinString> &params
	)
	{
		location_conf_[location.ToString()] = LocationConf(params);
	}

	Port VirtualServerConf::GetListenPort() const
	{
		return listen_port_;
	}

	std::string VirtualServerConf::GetServerName() const
	{
		return server_name_;
	}

	std::map<StatusCode, Path> VirtualServerConf::GetErrorPages() const
	{
		return error_pages_;
	}

	std::size_t VirtualServerConf::GetClientMaxBodySize() const
	{
		return client_max_body_size_;
	}

	std::map<Path, LocationConf> VirtualServerConf::GetLocationConfs() const
	{
		return location_conf_;
	}

	bool VirtualServerConf::operator==(const VirtualServerConf &rhs) const
	{
		return listen_port_ == rhs.listen_port_ && server_name_ == rhs.server_name_ &&
			   error_pages_ == rhs.error_pages_ &&
			   client_max_body_size_ == rhs.client_max_body_size_ &&
			   location_conf_ == rhs.location_conf_;
	}

	std::ostream &operator<<(std::ostream &os, const VirtualServerConf &conf)
	{
		os << "listen_port: " << conf.GetListenPort() << std::endl;
		os << "server_name: " << conf.GetServerName() << std::endl;
		std::map<StatusCode, Path> error_pages = conf.GetErrorPages();
		os << "error_pages: " << std::endl;
		for (std::map<StatusCode, Path>::const_iterator it = error_pages.begin();
			 it != error_pages.end();
			 ++it) {
			os << "  " << it->first << ": " << it->second << std::endl;
		}
		os << "client_max_body_size: " << conf.GetClientMaxBodySize() << std::endl;
		std::map<Path, LocationConf> location_confs = conf.GetLocationConfs();
		// os << "location_confs: " << std::endl;
		// for (std::map<Path, LocationConf>::const_iterator it = location_confs.begin();
		// 	 it != location_confs.end();
		// 	 ++it) {
		// 	os << "  " << it->first << ": " << it->second << std::endl;
		// }
		return os;
	}
} // namespace conf
