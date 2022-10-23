#include "server_confs.hpp"
#include "config_exceptions.hpp"
#include "thin_string.hpp"
#include "webserv_utils.hpp"

#include <list>
#include <map>
#include <stack>
#include <string>

namespace conf
{
	std::map<Port, VirtualServerConfs> ServerConfs::empty_map_;

	/**
	 * @brief Construct a new Server Confs
	 *
	 * @param config_file_path
	 * @throw conf::ConfigException
	 */
	ServerConfs::ServerConfs(std::string &config_file_path)
	{
		Result<std::string> res = utils::ReadFile(config_file_path);
		if (res.IsErr()) {
			throw ConfigException(res.Err());
		}
		std::string config_file_content = res.Val();

		std::vector<VirtualServerConf> v_servers = ParseConfigFile(config_file_content);
		for (std::vector<VirtualServerConf>::iterator it = v_servers.begin(); it != v_servers.end();
			 ++it) {
			Port port             = it->GetListenPort();
			Host host             = it->GetServerName();
			bool port_has_no_conf = virtual_servers_.find(port) == virtual_servers_.end();
			if (port_has_no_conf) {
				virtual_servers_[port];
			}
			virtual_servers_[port][host] = *it;
		}
	}

	// std::list<std::list<ThinString>> GetConfigStack(std::string &config_file_content)
	// {
	// 	typedef std::list<ThinString> Block;

	// 	ThinString       content(config_file_content);
	// 	std::list<Block> config;

	// 	std::stack<Block> stack;
	// 	stack.push(Block());
	// 	for (size_t i = 0; i < content.size(); ++i) {
	// 	}
	// 	return config;
	// }

	// std::size_t GetEndBracketPos(const ThinString &str, std::size_t start_pos)
	// {
	// 	std::size_t pos           = start_pos;
	// 	int         bracket_count = 0;
	// 	bool        is_end        = false;
	// 	while (pos < str.size()) {
	// 		if (str.at(pos) == '{') {
	// 			bracket_count++;
	// 		}
	// 		if (str.at(pos) == '}') {
	// 			bracket_count--;
	// 			if (bracket_count == 0) {
	// 				is_end = true;
	// 			}
	// 		}
	// 		if (is_end) {
	// 			if (str.at(pos) == '\n') {
	// 				return pos;
	// 			} else {
	// 				return std::string::npos;
	// 			}
	// 		}
	// 		pos++;
	// 	}
	// 	return std::string::npos;
	// }

	// std::list<ThinString> GetConfigList(const std::string &file_content)
	// {
	// 	std::list<ThinString> v_servers;
	// 	ThinString            content(file_content);

	// 	std::size_t start_pos = 0, end_pos = 0;
	// 	while (start_pos < content.size()) {
	// 		start_pos = content.find("server {");
	// 		if (start_pos != 0) {
	// 			throw ConfigException("Invalid server block");
	// 		}
	// 		end_pos = GetEndBracketPos(content, start_pos);
	// 		if (end_pos == std::string::npos) {
	// 			throw ConfigException("No end bracket");
	// 		}
	// 		v_servers.push_back(content.substr(start_pos, end_pos - start_pos + 1));

	// 		content = content.substr(end_pos + 1);
	// 	}
	// 	return v_servers;
	// }

	ServerConfs::~ServerConfs() {}

	/**
	 * @brief portに対応するstd::map<Host, VirtualServerConf>を返す
	 * @brief ない場合はemptyなmapを返す
	 *
	 * @param port
	 * @return const std::map<Host, VirtualServerConf>&
	 */
	VirtualServerConfs &ServerConfs::operator[](const Port &port)
	{
		if (virtual_servers_.find(port) == virtual_servers_.end()) {
			return empty_map_[port];
		}
		return virtual_servers_[port];
	}

	ThinString TrimWSLF(const ThinString &str)
	{
		return str.TrimLeft(" \t\n").TrimRight(" \t\n");
	}

	void WhenBraceOpen(
		std::vector<VirtualServerConf>       &v_servers,
		std::stack<ThinString>               &head_stack,
		std::stack<std::vector<ThinString> > &content_stack,
		const ThinString                     &content
	)
	{
		head_stack.push(TrimWSLF(content));
		content_stack.push(std::vector<ThinString>());
		if (!head_stack.empty() && head_stack.top() == "server") {
			v_servers.push_back(VirtualServerConf());
		}
	}

	void WhenBraceClose(
		std::vector<VirtualServerConf>       &v_servers,
		std::stack<ThinString>               &head_stack,
		std::stack<std::vector<ThinString> > &content_stack
	)
	{
		if (head_stack.empty() || content_stack.empty()) {
			throw ConfigException("Invalid config file");
		}
		if (head_stack.top() == "server") {
			v_servers.back().SetParams(content_stack.top());
		} else if (head_stack.top().StartWith("location")) {
			v_servers.back().AddLocation(head_stack.top(), content_stack.top());
		}
		head_stack.pop();
		content_stack.pop();
	}

	std::vector<VirtualServerConf> ParseConfigFile(const std::string &config_file_content)
	{
		std::vector<VirtualServerConf> v_servers;
		ThinString                     contents(config_file_content);

		std::stack<ThinString>               head_stack;
		std::stack<std::vector<ThinString> > content_stack;
		std::size_t                          start = 0;
		for (std::size_t i = 0; i < contents.size(); ++i) {
			switch (contents.at(i)) {
			case '{':
				WhenBraceOpen(
					v_servers, head_stack, content_stack, contents.substr(start, i - start)
				);
				start = i + 1;
				break;
			case ';':
				content_stack.top().push_back(TrimWSLF(contents.substr(start, i - start)));
				start = i + 1;
				break;
			case '}':
				WhenBraceClose(v_servers, head_stack, content_stack);
				start = i + 1;
				break;
			default:
				break;
			}
		}
		if (!head_stack.empty() || !content_stack.empty()) {
			throw ConfigException("Invalid config file");
		}

		return v_servers;
	}

} // namespace conf
