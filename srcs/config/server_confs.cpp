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
	/**
	 * @brief Construct a new Server Confs
	 *
	 * @param config_file_path
	 * @throw conf::ConfigException
	 */
	ServerConfs::ServerConfs(const std::string &config_file_path)
	{
		Result<std::string> res = utils::ReadFile(config_file_path);
		if (res.IsErr()) {
			throw ConfigException(res.Err());
		}
		std::string config_file_content = res.Val();

		confs_ = ParseConfigFile(config_file_content);
		CreatePortHostMap();
	}

	ServerConfs::ServerConfs(const std::string &file_content, bool is_test)
	{
		(void)is_test;
		confs_ = ParseConfigFile(file_content);
		CreatePortHostMap();
	}

	ServerConfs::ServerConfs(const ConfsStore &confs, const ConfsMap &confs_map)
	{
		confs_     = confs;
		confs_map_ = confs_map;
	}

	ServerConfs::~ServerConfs() {}

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
	Result<VirtualServerConfs &> ServerConfs::operator[](const Port &port)
	{
		try {
			return confs_map_[port];
		} catch (const std::out_of_range &e) {
			return Error("port not found");
		}
	}

	ThinString TrimWSLF(const ThinString &str)
	{
		return str.TrimLeft(" \t\n").TrimRight(" \t\n");
	}

	void WhenBraceOpen(
		std::vector<ServerConf> &v_servers, ParseStack &parse_stack, const ThinString &content
	)
	{
		parse_stack.push(TrimWSLF(content));
		if (parse_stack.TopHeader() == "server") {
			v_servers.push_back(ServerConf());
		}
	}

	void WhenBraceClose(std::vector<ServerConf> &v_servers, ParseStack &parse_stack)
	{
		if (parse_stack.empty()) {
			throw ConfigException("Invalid config file");
		}
		if (parse_stack.TopHeader() == "server") {
			v_servers.back().SetParams(parse_stack.TopContents());
		} else if (parse_stack.TopHeader().StartWith("location")) {
			v_servers.back().AddLocation(parse_stack.TopHeader(), parse_stack.TopContents());
		}
		parse_stack.pop();
	}

	std::vector<ServerConf> ParseConfigFile(const std::string &config_file_content)
	{
		std::vector<ServerConf> v_servers;
		ThinString              contents(config_file_content);

		ParseStack  parse_stack;
		std::size_t start = 0;
		for (std::size_t i = 0; i < contents.size(); ++i) {
			switch (contents.at(i)) {
			case '{':
				WhenBraceOpen(v_servers, parse_stack, contents.substr(start, i - start));
				start = i + 1;
				break;
			case ';':
				parse_stack.AddContent(TrimWSLF(contents.substr(start, i - start)));
				start = i + 1;
				break;
			case '}':
				WhenBraceClose(v_servers, parse_stack);
				start = i + 1;
				break;
			default:
				break;
			}
		}
		if (!parse_stack.empty()) {
			throw ConfigException("Invalid config file");
		}

		return v_servers;
	}

	void ServerConfs::CreatePortHostMap()
	{
		typedef std::vector<ServerConf>::iterator  ConfsItr;
		typedef std::vector<std::string>::iterator StrItr;

		for (ConfsItr it = confs_.begin(); it != confs_.end(); ++it) {

			ServerConf::ListenPort port = it->GetListenPort();

			for (StrItr port_it = port.begin(); port_it != port.end(); ++port_it) {

				ServerConf::ServerName hosts = it->GetServerName();
				for (StrItr host_it = hosts.begin(); host_it != hosts.end(); ++host_it) {

					confs_map_[*port_it].Add(*host_it, *it);
				}
			}
		}
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
