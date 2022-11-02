#include "server_conf.hpp"

#include "config_exceptions.hpp"
#include "parse_abnf_core_rules.hpp"
#include "webserv_utils.hpp"

namespace conf
{
	ServerConf::ServerConf(
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

	ServerConf::~ServerConf() {}

	void ServerConf::SetParams(const std::vector<ThinString> &params)
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
		if (listen_port_.empty()) {
			listen_port_.push_back("80");
		}
	}

	/**
	 * Add Params
	 */

	void ServerConf::AddListenPort(const std::vector<ThinString> &tokens)
	{
		for (size_t i = 1; i < tokens.size(); i++) {
			if (ABNF::IsDigitOnly(tokens[i])) {
				listen_port_.push_back(tokens[i].ToString());
			} else {
				throw ConfigException("Invalid config");
			}
		}
	}

	void ServerConf::AddServerName(const std::vector<ThinString> &tokens)
	{
		for (size_t i = 1; i < tokens.size(); i++) {
			server_name_.push_back(tokens[i].ToString());
		}
	}

	void ServerConf::AddErrorPages(const std::vector<ThinString> &tokens)
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

	void ServerConf::AddClientMaxBodySize(const std::vector<ThinString> &tokens)
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
			size = utils::stoul(num_part.ToString());
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

	void ServerConf::AddLocation(const ThinString &location, const std::vector<ThinString> &params)
	{
		std::vector<ThinString> splitted_location = Split(location, " ");
		if (splitted_location.size() != 2) {
			throw ConfigException("Invalid location");
		}
		Path pertern             = splitted_location[1].ToString();
		location_confs_[pertern] = LocationConf(params);
	}

	/**
	 * Getters
	 */
	const ServerConf::ListenPort &ServerConf::GetListenPort() const
	{
		return listen_port_;
	}

	const ServerConf::ServerName &ServerConf::GetServerName() const
	{
		return server_name_;
	}

	const ServerConf::ErrorPages &ServerConf::GetErrorPages() const
	{
		return error_pages_;
	}

	const ServerConf::ClientMaxBodySize &ServerConf::GetClientMaxBodySize() const
	{
		return client_max_body_size_;
	}

	const ServerConf::LocationConfs &ServerConf::GetLocationConfs() const
	{
		return location_confs_;
	}

	/**
	 * Operators
	 */

	bool ServerConf::operator==(const ServerConf &rhs) const
	{
		return listen_port_ == rhs.listen_port_ && server_name_ == rhs.server_name_ &&
			   error_pages_ == rhs.error_pages_ &&
			   client_max_body_size_ == rhs.client_max_body_size_ &&
			   location_confs_ == rhs.location_confs_;
	}

	std::ostream &operator<<(std::ostream &os, const ServerConf &conf)
	{
		os << "\nlisten: ";
		ServerConf::ListenPort ports = conf.GetListenPort();
		for (ServerConf::ListenPort::const_iterator it = ports.begin(); it != ports.end(); ++it) {
			os << *it << " ";
		}
		os << std::endl;

		os << "server_name: ";
		ServerConf::ServerName hosts = conf.GetServerName();
		for (ServerConf::ServerName::const_iterator it = hosts.begin(); it != hosts.end(); ++it) {
			os << *it << " ";
		}
		os << std::endl;

		os << "error_page: ";
		ServerConf::ErrorPages errors = conf.GetErrorPages();
		for (ServerConf::ErrorPages::const_iterator it = errors.begin(); it != errors.end(); ++it) {
			os << it->first << " " << it->second << " ";
		}
		os << std::endl;

		ServerConf::ClientMaxBodySize max_body_size = conf.GetClientMaxBodySize();
		os << "client_max_body_size: " << max_body_size.Value() << std::endl;

		os << "location: " << std::endl;
		ServerConf::LocationConfs locations = conf.GetLocationConfs();
		for (ServerConf::LocationConfs::const_iterator it = locations.begin();
			 it != locations.end();
			 ++it) {
			os << it->first << "{" << it->second << "}" << std::endl;
		}
		return os;
	}
} // namespace conf
