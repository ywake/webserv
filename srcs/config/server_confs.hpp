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
	  public:
		typedef std::vector<ServerConf>            ConfsStore;
		typedef std::map<Port, VirtualServerConfs> ConfsMap;

	  private:
		ConfsStore      confs_;
		ConfsMap        confs_map_;
		static ConfsMap empty_map_;

	  public:
		ServerConfs(const std::string &config_file_path);

		// for test. actualy値のため 第2引数はコンストラクタの区別のためだけに存在
		ServerConfs(const std::string &file_content, bool is_test);

		// for test. expect値のため
		ServerConfs(const ConfsStore &confs, const ConfsMap &confs_map);

		~ServerConfs();

		bool operator==(const ServerConfs &rhs) const;
		/**
		 * @brief portに対応するstd::map<Host, ServerConf>を返す
		 * @brief ない場合はemptyなmapを返す
		 *
		 * @param port
		 * @return const std::map<Host, ServerConf>&
		 */
		VirtualServerConfs &operator[](const Port &port);

		void Print(std::ostream &os) const;

	  private:
		void PortHostMapping();
	};

	std::ostream &operator<<(std::ostream &out, const ServerConfs &server_confs);

	// std::list<ThinString> GetConfigList(const std::string &file_content);
	std::vector<ServerConf> ParseConfigFile(const std::string &config_file_content);
} // namespace conf

#endif
