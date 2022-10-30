#include "server_conf.hpp"

#include "config_exceptions.hpp"
#include "parse_abnf_core_rules.hpp"
#include "webserv_utils.hpp"

namespace conf
{
	VirtualServerConf::VirtualServerConf(
		std::vector<Port>            listen_port,
		std::vector<std::string>     server_name,
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
				for (size_t i = 1; i < split.size(); i++) {
					if (ABNF::IsDigitOnly(split[i])) {
						listen_port_.push_back(split[i].ToString());
					} else {
						throw ConfigException("Invalid config");
					}
				}
			} else if (split[0] == "server_name") {
				for (size_t i = 1; i < split.size(); i++) {
					server_name_.push_back(split[i].ToString());
				}
			} else if (split[0] == "error_page") {
				if (split.size() != 3) {
					throw ConfigException("Invalid error_page");
				}
				if (ABNF::IsDigitOnly(split[1])) {
					error_pages_[split[1].ToString()] = split[2].ToString();
				} else {
					throw ConfigException("Invalid error_page");
				}
			} else if (split[0] == "client_max_body_size") {
				if (split.size() != 2) {
					throw ConfigException("Invalid client_max_body_size");
				}
				client_max_body_size_ = std::stoul(split[1].ToString());
			} else {
				throw ConfigException("Invalid config");
			}
		}
		if (listen_port_.empty()) {
			listen_port_.push_back("80");
		}
	}

	void VirtualServerConf::AddLocation(
		const ThinString &location, const std::vector<ThinString> &params
	)
	{
		location_conf_[location.ToString()] = LocationConf(params);
	}

	std::vector<Port> VirtualServerConf::GetListenPort() const
	{
		return listen_port_;
	}

	std::vector<std::string> VirtualServerConf::GetServerName() const
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
		os << "\nlisten: ";
		std::vector<Port> ports = conf.GetListenPort();
		for (std::vector<Port>::const_iterator it = ports.begin(); it != ports.end(); ++it) {
			os << *it << " ";
		}
		os << std::endl;
		os << "server_name: ";
		std::vector<std::string> hosts = conf.GetServerName();
		for (std::vector<std::string>::const_iterator it = hosts.begin(); it != hosts.end(); ++it) {
			os << *it << " ";
		}
		os << std::endl;
		os << "error_page: ";
		std::map<StatusCode, Path> errors = conf.GetErrorPages();
		for (std::map<StatusCode, Path>::const_iterator it = errors.begin(); it != errors.end();
			 ++it) {
			os << it->first << " " << it->second << " ";
		}
		os << std::endl;
		os << "client_max_body_size: " << conf.GetClientMaxBodySize() << std::endl;
		os << "location: " << std::endl;
		std::map<Path, LocationConf> locations = conf.GetLocationConfs();
		for (std::map<Path, LocationConf>::const_iterator it = locations.begin();
			 it != locations.end();
			 ++it) {
			os << it->first << std::endl;
			// os << it->second << std::endl;
		}
		return os;
	}
} // namespace conf
