#include "server_conf.hpp"

#include "config_exceptions.hpp"
#include "parse_abnf_core_rules.hpp"
#include "webserv_utils.hpp"

namespace conf
{
	VirtualServerConf::VirtualServerConf(
		ListenPort        listen_port,
		ServerName        server_name,
		ErrorPages        error_pages,
		ClientMaxBodySize client_max_body_size,
		LocationConfs     location_confs
	)
		: listen_port_(listen_port),
		  server_name_(server_name),
		  error_pages_(error_pages),
		  client_max_body_size_(client_max_body_size),
		  location_confs_(location_confs)
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
				AddErrorPages(tokens);
			} else if (tokens[0] == "client_max_body_size") {
				AddClientMaxBodySize(tokens);
			} else {
				throw ConfigException("Invalid config");
			}
		}
		if (listen_port_.Value().empty()) {
			listen_port_.Value().push_back("80");
		}
	}

	/**
	 * Add Params
	 */

	void VirtualServerConf::AddListenPort(const std::vector<ThinString> &tokens)
	{
		for (size_t i = 1; i < tokens.size(); i++) {
			if (ABNF::IsDigitOnly(tokens[i])) {
				listen_port_.Value().push_back(tokens[i].ToString());
			} else {
				throw ConfigException("Invalid config");
			}
		}
	}

	void VirtualServerConf::AddServerName(const std::vector<ThinString> &tokens)
	{
		for (size_t i = 1; i < tokens.size(); i++) {
			server_name_.Value().push_back(tokens[i].ToString());
		}
	}

	void VirtualServerConf::AddErrorPages(const std::vector<ThinString> &tokens)
	{
		if (tokens.size() != 3) {
			throw ConfigException("Invalid error_page");
		}
		if (ABNF::IsDigitOnly(tokens[1])) {
			error_pages_.Value()[tokens[1].ToString()] = tokens[2].ToString();
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
		std::vector<ThinString> splitted_location = Split(location, " ");
		if (splitted_location.size() != 2) {
			throw ConfigException("Invalid location");
		}
		Path pertern                     = splitted_location[1].ToString();
		location_confs_.Value()[pertern] = LocationConf(params);
	}

	/**
	 * Getters
	 */
	const VirtualServerConf::ListenPort &VirtualServerConf::GetListenPort() const
	{
		return listen_port_;
	}

	const VirtualServerConf::ServerName &VirtualServerConf::GetServerName() const
	{
		return server_name_;
	}

	const VirtualServerConf::ErrorPages &VirtualServerConf::GetErrorPages() const
	{
		return error_pages_;
	}

	const VirtualServerConf::ClientMaxBodySize &VirtualServerConf::GetClientMaxBodySize() const
	{
		return client_max_body_size_;
	}

	const VirtualServerConf::LocationConfs &VirtualServerConf::GetLocationConfs() const
	{
		return location_confs_;
	}

	/**
	 * Operators
	 */

	bool VirtualServerConf::operator==(const VirtualServerConf &rhs) const
	{
		return listen_port_ == rhs.listen_port_ && server_name_ == rhs.server_name_ &&
			   error_pages_ == rhs.error_pages_ &&
			   client_max_body_size_ == rhs.client_max_body_size_ &&
			   location_confs_ == rhs.location_confs_;
	}

	std::ostream &operator<<(std::ostream &os, const VirtualServerConf &conf)
	{
		os << "\nlisten: ";
		VirtualServerConf::ListenPort ports     = conf.GetListenPort();
		std::vector<conf::Port>       port_list = ports.Value();
		for (std::vector<Port>::const_iterator it = port_list.begin(); it != port_list.end();
			 ++it) {
			os << *it << " ";
		}
		os << std::endl;

		os << "server_name: ";
		VirtualServerConf::ServerName hosts     = conf.GetServerName();
		std::vector<conf::Port>       host_list = hosts.Value();
		for (std::vector<std::string>::const_iterator it = host_list.begin(); it != host_list.end();
			 ++it) {
			os << *it << " ";
		}
		os << std::endl;

		os << "error_page: ";
		VirtualServerConf::ErrorPages          errors    = conf.GetErrorPages();
		std::map<conf::StatusCode, conf::Path> error_map = errors.Value();
		for (std::map<StatusCode, Path>::const_iterator it = error_map.begin();
			 it != error_map.end();
			 ++it) {
			os << it->first << " " << it->second << " ";
		}
		os << std::endl;

		VirtualServerConf::ClientMaxBodySize max_body_size = conf.GetClientMaxBodySize();
		os << "client_max_body_size: " << max_body_size.Value() << std::endl;

		os << "location: " << std::endl;
		VirtualServerConf::LocationConfs locations    = conf.GetLocationConfs();
		std::map<Path, LocationConf>     location_map = locations.Value();
		for (std::map<Path, LocationConf>::const_iterator it = location_map.begin();
			 it != location_map.end();
			 ++it) {
			os << it->first << "{" << it->second << "}" << std::endl;
		}
		return os;
	}
} // namespace conf
