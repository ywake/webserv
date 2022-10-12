#ifndef VIRTUAL_SERVER_CONFS_HPP
#define VIRTUAL_SERVER_CONFS_HPP

#include <map>

#include "config_typedef.hpp"
#include "virtual_server_conf.hpp"

namespace conf
{
	class VirtualServerConfs
	{
	  private:
		/* data */
		Host                              default_host_;
		std::map<Host, VirtualServerConf> server_confs_;

	  public:
		VirtualServerConfs(/* args */);
		~VirtualServerConfs();

		/**
		 * @brief hostに対応するVirtualServerConfを返す。
		 * @brief ない場合はdefault_hostのVirtualServerConfを返す。
		 * @brief
		 * defalut_hostもない場合はデフォルトコンストラクタで作成したVirtualServerConfを返す。
		 *
		 * @param host
		 * @return const VirtualServerConf&
		 */
		const VirtualServerConf &operator[](const Host &host);
	};

	VirtualServerConfs::VirtualServerConfs(/* args */) {}

	VirtualServerConfs::~VirtualServerConfs() {}
} // namespace conf

#endif
