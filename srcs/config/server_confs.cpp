#include "server_confs.hpp"
#include "config_exceptions.hpp"
#include "parse_stack.hpp"
#include "thin_string.hpp"
#include "webserv_utils.hpp"

#include <list>
#include <map>
#include <stack>
#include <string>

namespace conf
{

	ServerConfs::ServerConfs() : confs_(), confs_map_() {}

	ServerConfs::ServerConfs(const ServerConfs &other)
	{
		*this = other;
	}

	/**
	 * @brief Construct a new Server Confs
	 *
	 * @param config_file_path
	 * @throw conf::ConfigException
	 */
	ServerConfs ServerConfs::FromFilePath(const std::string &config_file_path)
	{
		Result<std::string> res = utils::ReadFile(config_file_path);
		if (res.IsErr()) {
			throw ConfigException(res.ErrMsg());
		}
		std::string config_file_content = res.Val();
		return ServerConfs(config_file_content);
	}

	ServerConfs::ServerConfs(const std::string &config_file_content)
	{
		confs_ = ParseConfigFile(config_file_content);
		if (confs_.empty()) {
			throw ConfigException("Empty config");
		}
		CreatePortHostMap();
	}

	ServerConfs::ServerConfs(const ConfsStore &confs, const ConfsMap &confs_map)
		: confs_(confs), confs_map_(confs_map)
	{}

	ServerConfs::~ServerConfs() {}

	ServerConfs &ServerConfs::operator=(const ServerConfs &other)
	{
		confs_ = other.confs_;
		CreatePortHostMap();
		return *this;
	}

	bool ServerConfs::operator==(const ServerConfs &rhs) const
	{
		return confs_ == rhs.confs_ && confs_map_ == rhs.confs_map_;
	}

	/**
	 * @brief portに対応するstd::map<Host, ServerConf>を返す
	 * @brief ない場合はemptyなmapを返す
	 *
	 * @param port
	 * @return const std::map<Host, ServerConf>&
	 */
	Result<const VirtualServerConfs &> ServerConfs::operator[](const Port &port) const
	{
		try {
			return confs_map_.at(port);
		} catch (const std::out_of_range &e) {
			return Error("port not found");
		}
	}

	ThinString TrimWSLF(const ThinString &str)
	{
		return str.TrimLeft(" \t\n").TrimRight(" \t");
	}

	void WhenBraceOpen(
		std::vector<ServerConf> &v_servers, ParseStack &parse_stack, const ThinString &content
	)
	{
		parse_stack.push(content);
		if (parse_stack.TopHeader() == "server") {
			v_servers.push_back(ServerConf());
		}
	}

	void WhenBraceClose(
		std::vector<ServerConf> &v_servers, ParseStack &parse_stack, const ThinString &content
	)
	{
		if (parse_stack.empty() || !content.empty()) {
			throw ConfigException("Invalid config file");
		}
		if (parse_stack.TopHeader() == "server") {
			v_servers.back().SetParams(parse_stack.TopContents());
			if (v_servers.back().GetDefaultRoot().empty()) {
				throw ConfigException("Invalid server config: root is not set");
			}
		} else if (parse_stack.TopHeader().StartWith("location ")) {
			v_servers.back().AddLocation(
				parse_stack.TopHeader(),
				v_servers.back().GetDefaultRoot().Value(),
				parse_stack.TopContents()
			);
		}

		parse_stack.pop();
	}

	std::vector<ServerConf> ParseConfigFile(const std::string &config_file_content)
	{
		std::vector<ServerConf> v_servers;
		ThinString              file(config_file_content);

		ParseStack  parse_stack;
		std::size_t start = 0;
		for (std::size_t i = 0; i < file.size(); ++i) {
			char now      = file.at(i);
			bool is_delim = now == '{' || now == '}' || now == ';';
			if (!is_delim) {
				continue;
			}

			ThinString content = TrimWSLF(file.substr(start, i - start));
			switch (now) {
			case '{':
				WhenBraceOpen(v_servers, parse_stack, content);
				break;
			case ';':
				parse_stack.AddContent(content);
				break;
			case '}':
				WhenBraceClose(v_servers, parse_stack, content);
				break;
			}
			start = i + 1;
		}
		if (!parse_stack.empty()) {
			throw ConfigException("Invalid config file");
		}

		return v_servers;
	}

	void ServerConfs::CreatePortHostMap()
	{
		typedef std::vector<ServerConf>::const_iterator  ConfsItr;
		typedef std::vector<std::string>::const_iterator StrItr;

		// ConfArray loop
		for (ConfsItr it = confs_.begin(); it != confs_.end(); ++it) {
			const ServerConf             &serverConf = *it;
			const ServerConf::ListenPort &ports      = serverConf.GetListenPort();
			// PortArray loop
			for (StrItr port_it = ports.begin(); port_it != ports.end(); ++port_it) {
				const Port                   &port  = *port_it;
				const ServerConf::ServerName &hosts = serverConf.GetServerName();
				// HostArray loop
				for (StrItr host_it = hosts.begin(); host_it != hosts.end(); ++host_it) {
					RegisterServerConf(port, *host_it, serverConf);
				}
			}
		}
	}

	void ServerConfs::RegisterServerConf(const Port &port, const Host &host, const ServerConf &conf)
	{
		typedef ServerConfs::ConfsMap::const_iterator ConfsMapItr;

		ConfsMapItr confs_map_it = confs_map_.find(port);
		if (confs_map_it == confs_map_.end()) {
			confs_map_.insert(std::pair<Port, VirtualServerConfs>(port, VirtualServerConfs(host)));
		}
		confs_map_[port].Add(host, conf);
	}

	ServerConfs::ConfsMap::const_iterator ServerConfs::begin() const
	{
		return confs_map_.begin();
	}

	ServerConfs::ConfsMap::const_iterator ServerConfs::end() const
	{
		return confs_map_.end();
	}

	void ServerConfs::Print(std::ostream &os) const
	{
		for (ConfsMap::const_iterator it = confs_map_.begin(); it != confs_map_.end(); ++it) {
			os << "▼ ▼ ▼ port: " << it->first << "▼ ▼ ▼ " << std::endl;
			os << it->second;
			os << "▲ ▲ ▲ port: " << it->first << "▲ ▲ ▲ " << std::endl;
		}
	}

	std::ostream &operator<<(std::ostream &os, const ServerConfs &confs)
	{
		confs.Print(os);
		return os;
	}

} // namespace conf
