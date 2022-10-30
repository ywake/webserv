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

			std::vector<ThinString> tokens = Split(*it, " ");
			if (tokens.size() < 2) {
				throw ConfigException("Invalid config");
			}
			if (tokens[0] == "listen") {
				AddListenPort(tokens);
			} else if (tokens[0] == "server_name") {
				AddServerName(tokens);
			} else if (tokens[0] == "error_page") {
				AddErrorPage(tokens);
			} else if (tokens[0] == "client_max_body_size") {
				AddClientMaxBodySize(tokens);
			} else {
				throw ConfigException("Invalid config");
			}
		}
		if (listen_port_.empty()) {
			listen_port_.push_back("80");
		}
	}

	/**
	 * Add Params
	 */

	void VirtualServerConf::AddListenPort(const std::vector<ThinString> &tokens)
	{
		for (size_t i = 1; i < tokens.size(); i++) {
			if (ABNF::IsDigitOnly(tokens[i])) {
				listen_port_.push_back(tokens[i].ToString());
			} else {
				throw ConfigException("Invalid config");
			}
		}
	}

	void VirtualServerConf::AddServerName(const std::vector<ThinString> &tokens)
	{
		for (size_t i = 1; i < tokens.size(); i++) {
			server_name_.push_back(tokens[i].ToString());
		}
	}

	void VirtualServerConf::AddErrorPage(const std::vector<ThinString> &tokens)
	{
		if (tokens.size() != 3) {
			throw ConfigException("Invalid error_page");
		}
		if (ABNF::IsDigitOnly(tokens[1])) {
			error_pages_[tokens[1].ToString()] = tokens[2].ToString();
		} else {
			throw ConfigException("Invalid error_page");
		}
	}

	Result<std::size_t> GetUnit(char unit)
	{
		switch (unit) {
		case 'k':
		case 'K':
			return 1024UL;
		case 'm':
		case 'M':
			return 1024UL * 1024UL;
		case 'g':
		case 'G':
			return 1024UL * 1024UL * 1024UL;
		default:
			return Error("Invalid unit");
		}
	}

	void VirtualServerConf::AddClientMaxBodySize(const std::vector<ThinString> &tokens)
	{
		if (tokens.size() != 2) {
			throw ConfigException("Invalid client_max_body_size");
		}
		char last_char    = tokens[1].at(tokens[1].size() - 1);
		bool last_is_unit = !std::isdigit(last_char);

		std::size_t size = 0;

		ThinString num_part;
		if (last_is_unit) {
			num_part = tokens[1].substr(0, tokens[1].size() - 1);
		} else {
			num_part = tokens[1];
		}
		if (ABNF::IsDigitOnly(num_part)) {
			size = std::stoul(num_part.ToString());
		} else {
			throw ConfigException("Invalid client_max_body_size");
		}

		std::size_t unit_size = 1;
		if (last_is_unit) {
			Result<std::size_t> unit = GetUnit(last_char);
			if (unit.IsErr()) {
				throw ConfigException("Invalid client_max_body_size");
			}
			unit_size = unit.Val();
		}

		client_max_body_size_ = size * unit_size;
	}

	void VirtualServerConf::AddLocation(
		const ThinString &location, const std::vector<ThinString> &params
	)
	{
		location_conf_[location.ToString()] = LocationConf(params);
	}

	/**
	 * Getters
	 */
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

	/**
	 * Operators
	 */

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
