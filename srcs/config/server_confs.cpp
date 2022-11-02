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

		confs_ = ParseConfigFile(config_file_content);
		PortHostMapping();
	}

	ServerConfs::~ServerConfs() {}

	/**
	 * @brief portに対応するstd::map<Host, ServerConf>を返す
	 * @brief ない場合はemptyなmapを返す
	 *
	 * @param port
	 * @return const std::map<Host, ServerConf>&
	 */
	VirtualServerConfs &ServerConfs::operator[](const Port &port)
	{
		if (conf_maps_.find(port) == conf_maps_.end()) {
			return empty_map_[port];
		}
		return conf_maps_[port];
	}

	ThinString TrimWSLF(const ThinString &str)
	{
		return str.TrimLeft(" \t\n").TrimRight(" \t\n");
	}

	void WhenBraceOpen(
		std::vector<ServerConf>              &v_servers,
		std::stack<ThinString>               &head_stack,
		std::stack<std::vector<ThinString> > &content_stack,
		const ThinString                     &content
	)
	{
		head_stack.push(TrimWSLF(content));
		content_stack.push(std::vector<ThinString>());
		if (!head_stack.empty() && head_stack.top() == "server") {
			v_servers.push_back(ServerConf());
		}
	}

	void WhenBraceClose(
		std::vector<ServerConf>              &v_servers,
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

	std::vector<ServerConf> ParseConfigFile(const std::string &config_file_content)
	{
		std::vector<ServerConf> v_servers;
		ThinString              contents(config_file_content);

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

	void ServerConfs::PortHostMapping()
	{
		typedef std::vector<ServerConf>::iterator  ConfsItr;
		typedef std::vector<std::string>::iterator StrItr;

		for (ConfsItr it = confs_.begin(); it != confs_.end(); ++it) {

			ServerConf::ListenPort port  = it->GetListenPort();
			std::vector<Port>      ports = port.Value();

			for (StrItr port_it = ports.begin(); port_it != ports.end(); ++port_it) {

				ServerConf::ServerName server_name = it->GetServerName();
				std::vector<Host>      hosts       = server_name.Value();
				for (StrItr host_it = hosts.begin(); host_it != hosts.end(); ++host_it) {

					bool port_has_no_conf = conf_maps_.find(*port_it) == conf_maps_.end();
					if (port_has_no_conf) {
						conf_maps_[*port_it];
					}
					conf_maps_[*port_it][*host_it] = &(*it);
				}
			}
		}
	}

} // namespace conf
