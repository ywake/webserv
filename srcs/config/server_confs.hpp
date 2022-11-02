#ifndef VIRTUAL_SERVERS_HPP
#define VIRTUAL_SERVERS_HPP

#include <map>
#include <string>

#include "config_typedef.hpp"
#include "host_port.hpp"
#include "result.hpp"
#include "virtual_server_confs.hpp"

namespace conf
{
	class ServerConfs
	{
		static std::map<Port, VirtualServerConfs> empty_map_;

	  private:
		std::vector<ServerConf>            confs_;
		std::map<Port, VirtualServerConfs> conf_maps_;

	  public:
		ServerConfs(std::string &config_file_path);
		~ServerConfs();

		/**
		 * @brief portに対応するstd::map<Host, ServerConf>を返す
		 * @brief ない場合はemptyなmapを返す
		 *
		 * @param port
		 * @return const std::map<Host, ServerConf>&
		 */
		VirtualServerConfs &operator[](const Port &port);

	  private:
		void PortHostMapping();
	};

	// std::list<ThinString> GetConfigList(const std::string &file_content);
	std::vector<ServerConf> ParseConfigFile(const std::string &config_file_content);
} // namespace conf

#endif
