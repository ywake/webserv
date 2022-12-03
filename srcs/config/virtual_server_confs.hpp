#ifndef VIRTUAL_SERVER_CONFS_HPP
#define VIRTUAL_SERVER_CONFS_HPP

#include "config_typedef.hpp"
#include "emptiable.hpp"
#include "result.hpp"
#include "server_conf.hpp"
#include <map>

namespace conf
{
	class VirtualServerConfs
	{
	  public:
		typedef std::map<Host, const ServerConf *> HostMap;

	  private:
		/* data */
		Host    default_host_;
		HostMap server_confs_;

	  public:
		VirtualServerConfs();
		VirtualServerConfs(const Host &default_host);

		// for test
		VirtualServerConfs(const Host &default_host, const HostMap &server_confs);
		~VirtualServerConfs();

		bool operator==(const VirtualServerConfs &rhs) const;
		/**
		 * @brief hostに対応するVirtualServerConfを返す。
		 * @brief ない場合はdefault_hostのVirtualServerConfを返す。
		 * @brief
		 * defalut_hostもない場合はデフォルトコンストラクタで作成したVirtualServerConfを返す。
		 *
		 * @param host
		 * @return const ServerConf*
		 */
		const ServerConf &operator[](const Host &host) const;

		void Add(const Host &host, const ServerConf &server_conf);

		void Print(std::ostream &os) const;
	};

	std::ostream &operator<<(std::ostream &os, const VirtualServerConfs &virtual_server_confs);
} // namespace conf

#endif
