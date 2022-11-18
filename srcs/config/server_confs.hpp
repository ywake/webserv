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
		ConfsStore confs_;
		ConfsMap   confs_map_;

	  public:
		static ServerConfs FromFilePath(const std::string &config_file_path);
		ServerConfs(const std::string &config_file_content);

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
		Result<const VirtualServerConfs &> operator[](const Port &port);
		ConfsMap::const_iterator           begin() const;
		ConfsMap::const_iterator           end() const;

		void Print(std::ostream &os) const;

	  private:
		void CreatePortHostMap();
	};

	std::ostream &operator<<(std::ostream &out, const ServerConfs &server_confs);

	// std::list<ThinString> GetConfigList(const std::string &file_content);
	std::vector<ServerConf> ParseConfigFile(const std::string &config_file_content);
} // namespace conf

#endif
