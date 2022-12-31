#include "server_conf.hpp"

#include "config_exceptions.hpp"
#include "parse_abnf_core_rules.hpp"
#include "webserv_utils.hpp"

#include <cassert>

namespace conf
{
	bool IsInRange(const ThinString &str, std::size_t min, std::size_t max);
	bool IsValidListenPort(ThinString str);
	bool IsValidErrorPage(ThinString key, ThinString value);

	const ServerConf::ServerName        ServerConf::kDefaultServerName = ServerConf::ServerName();
	const ServerConf::ErrorPages        ServerConf::kDefaultErrorPages = ServerConf::ErrorPages();
	const ServerConf::ClientMaxBodySize ServerConf::kDefaultClientMaxBodySize = 1 << 20;
	// ListenPort({"80"})がC++98で出来ないので
	static const char           *kDefaultListenPortArray[]      = {"80"};
	const ServerConf::ListenPort ServerConf::kDefaultListenPort = ServerConf::ListenPort(
		kDefaultListenPortArray, kDefaultListenPortArray + ARRAY_SIZE(kDefaultListenPortArray)
	);

	ServerConf::ServerConf(
		Root              root,
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
		  location_confs_(location_confs),
		  default_root_(root),
		  default_location_conf_(LocationConf(
			  LocationConf::PathPattern(),
			  LocationConf::kPrefix,
			  LocationConf::AllowMethods(),
			  LocationConf::Redirect(),
			  LocationConf::Root(),
			  LocationConf::IndexFiles(),
			  LocationConf::AutoIndex(),
			  LocationConf::CgiPath(),
			  &default_root_.Value()
		  ))
	{}

	ServerConf::ServerConf(const ServerConf &other)
	{
		*this = other;
	}

	ServerConf::~ServerConf() {}

	void ServerConf::SetParams(const std::vector<ThinString> &params)
	{
		for (std::vector<ThinString>::const_iterator it = params.begin(); it != params.end();
			 ++it) {

			std::vector<ThinString> tokens = utils::TrimEmpty(Split(*it, " "));
			if (tokens.size() < 2) {
				throw ConfigException("Invalid server config: invalid size");
			}
			if (tokens[0] == "listen") {
				AddListenPort(tokens);
			} else if (tokens[0] == "server_name") {
				AddServerName(tokens);
			} else if (tokens[0] == "error_page") {
				AddErrorPages(tokens);
			} else if (tokens[0] == "client_max_body_size") {
				AddClientMaxBodySize(tokens);
			} else if (tokens[0] == "root") {
				AddRoot(tokens);
			} else {
				throw ConfigException("Invalid server config: invalid directive");
			}
		}
	}

	/**
	 * Add Params
	 */

	// 1~65535までならOK
	void ServerConf::AddListenPort(const std::vector<ThinString> &tokens)
	{
		for (size_t i = 1; i < tokens.size(); i++) {
			if (IsValidListenPort(tokens[i])) {
				listen_port_.push_back(tokens[i].ToString());
			} else {
				throw ConfigException("Invalid listen port");
			}
		}
	}

	bool IsValidListenPort(ThinString str)
	{
		if (!ABNF::IsDigitOnly(str)) {
			return false;
		}
		if (IsInRange(str, 1, 65535)) {
			return true;
		}
		return false;
	}

	bool IsInRange(const ThinString &str, std::size_t min, std::size_t max)
	{
		Result<std::size_t> port = utils::StrToUnsignedLong(str.ToString());
		if (port.IsErr()) {
			return false;
		}
		return min <= port.Val() && port.Val() <= max;
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
		if (IsValidErrorPage(tokens[1], tokens[2])) {
			error_pages_[tokens[1].ToString()] = tokens[2].ToString();
		} else {
			throw ConfigException("Invalid error_page");
		}
	}

	bool IsValidErrorPage(ThinString key, ThinString value)
	{
		if (key.size() != 3 || !ABNF::IsDigitOnly(key) || !IsInRange(key, 300, 599)) {
			return false;
		}
		if (value.empty() || value.at(0) != '/') {
			return false;
		}
		return true;
	}

	std::pair<ThinString, char> GetNumberAndUnit(ThinString str)
	{
		assert(!str.empty());
		std::pair<ThinString, char> ret;

		char last_char = str.at(str.size() - 1);
		if (std::isdigit(last_char)) {
			ret.first  = str;
			ret.second = 'b';
		} else {
			ret.first  = str.substr(0, str.size() - 1);
			ret.second = last_char;
		}
		return ret;
	}

	Result<std::size_t> GetUnit(char unit)
	{
		switch (unit) {
		case 'b':
		case 'B':
			return 1UL;
		case 'k':
		case 'K':
			return 1UL << 10;
		case 'm':
		case 'M':
			return 1UL << 20;
		// case 'g':
		// case 'G':
		// 	return 1UL << 30;
		default:
			return Error("Invalid unit");
		}
	}

	void ServerConf::AddClientMaxBodySize(const std::vector<ThinString> &tokens)
	{
		if (tokens.size() != 2) {
			throw ConfigException("Invalid client_max_body_size");
		}

		std::pair<ThinString, char> number_unit_pair = GetNumberAndUnit(tokens[1]);

		Result<std::size_t> res = utils::StrToUnsignedLong(number_unit_pair.first.ToString());
		if (res.IsErr()) {
			throw ConfigException("Invalid client_max_body_size");
		}
		std::size_t size = res.Val();

		Result<std::size_t> unit = GetUnit(number_unit_pair.second);
		if (unit.IsErr()) {
			throw ConfigException("Invalid client_max_body_size");
		}
		std::size_t unit_size = unit.Val();

		client_max_body_size_ = size * unit_size;
		if (size != client_max_body_size_.Value() / unit_size || client_max_body_size_ == 0 ||
			client_max_body_size_.Value() > kMaxClientBodySize) {
			throw ConfigException("Invalid client_max_body_size");
		}
	}

	Path GetPathPattern(const std::vector<ThinString> &tokens)
	{
		int path_index = tokens.size() == 3 ? 2 : 1;
		return tokens[path_index].ToString();
	}

	LocationConf::MatchPattern GetMatchPattern(const std::vector<ThinString> &tokens)
	{
		LocationConf::MatchPattern match_pattern = LocationConf::kPrefix;
		if (tokens.size() == 3) {
			if (tokens[1] == "=") {
				match_pattern = LocationConf::kExact;
			} else if (tokens[1] == "^") {
				match_pattern = LocationConf::kPrefix;
			} else if (tokens[1] == "$") {
				match_pattern = LocationConf::kSuffix;
			} else {
				throw ConfigException("Invalid location");
			}
		}
		return match_pattern;
	}

	void ServerConf::AddLocation(
		const ThinString &location, const Path &default_root, const std::vector<ThinString> &params
	)
	{
		std::vector<ThinString> splitted_location = utils::TrimEmpty(Split(location, " "));

		bool is_size2or3 = splitted_location.size() == 2 || splitted_location.size() == 3;
		if (!is_size2or3) {
			throw ConfigException("Invalid location");
		}
		if (splitted_location[0] != "location") {
			throw ConfigException("Invalid location");
		}

		Path                       path_pattern  = GetPathPattern(splitted_location);
		LocationConf::MatchPattern match_pattern = GetMatchPattern(splitted_location);

		location_confs_.push_back(LocationConf(path_pattern, match_pattern, default_root, params));
	}

	void ServerConf::AddRoot(const std::vector<ThinString> &tokens)
	{
		if (tokens.size() != 2) {
			throw ConfigException("Invalid root");
		}
		default_root_ = tokens[1].ToString();
		if (!default_root_.Value().empty() && default_root_.Value()[0] != '/') {
			throw ConfigException("Invalid root");
		}
	}

	/**
	 * Getters
	 */
	const ServerConf::ListenPort &ServerConf::GetListenPort() const
	{
		if (listen_port_.empty()) {
			return kDefaultListenPort;
		}
		return listen_port_;
	}

	const ServerConf::ServerName &ServerConf::GetServerName() const
	{
		if (server_name_.empty()) {
			return kDefaultServerName;
		}
		return server_name_;
	}

	const ServerConf::ErrorPages &ServerConf::GetErrorPages() const
	{
		if (error_pages_.empty()) {
			return kDefaultErrorPages;
		}
		return error_pages_;
	}

	const std::size_t &ServerConf::GetClientMaxBodySize() const
	{
		if (client_max_body_size_.empty()) {
			return kDefaultClientMaxBodySize.Value();
		}
		return client_max_body_size_.Value();
	}

	const ServerConf::LocationConfs &ServerConf::GetLocationConfs() const
	{
		return location_confs_;
	}

	const Path &ServerConf::GetRoot(const Path &uri_path) const
	{
		Result<const LocationConf &> matched_location = FindMatchingLocationConf(uri_path);
		if (matched_location.IsErr()) {
			return default_root_.Value();
		}
		Emptiable<Path> location_root = matched_location.Val().GetRoot();
		return location_root.empty() ? default_root_.Value() : location_root.Value();
	}

	const ServerConf::Root &ServerConf::GetDefaultRoot() const
	{
		return default_root_;
	}

	/**
	 * Methods
	 */
	const LocationConf &ServerConf::FindMatchingLocationConf(const Path &uri_path) const
	{
		for (LocationConfs::const_iterator it = location_confs_.begin();
			 it != location_confs_.end();
			 ++it) {
			if (it->IsMatch(uri_path)) {
				return *it;
			}
		}
		return default_location_conf_;
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

	bool ServerConf::operator!=(const ServerConf &rhs) const
	{
		return !(rhs == *this);
	}

	ServerConf &ServerConf::operator=(const ServerConf &rhs)
	{
		if (this == &rhs) {
			return *this;
		}
		listen_port_           = rhs.listen_port_;
		server_name_           = rhs.server_name_;
		error_pages_           = rhs.error_pages_;
		client_max_body_size_  = rhs.client_max_body_size_;
		location_confs_        = rhs.location_confs_;
		default_root_          = rhs.default_root_;
		default_location_conf_ = rhs.default_location_conf_;
		default_location_conf_.SetDefaultRoot(default_root_.Value());
		for (LocationConfs::iterator it = location_confs_.begin(); it != location_confs_.end();
			 ++it) {
			LocationConf &location_conf = *it;
			location_conf.SetDefaultRoot(default_root_.Value());
		}
		return *this;
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
			os << "{" << *it << "}" << std::endl;
		}
		return os;
	}
} // namespace conf
