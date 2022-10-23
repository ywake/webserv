#ifndef VIRTUAL_SERVER_CONFS_HPP
#define VIRTUAL_SERVER_CONFS_HPP

#include "config_typedef.hpp"
#include "server_conf.hpp"
#include <map>

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
		VirtualServerConf &operator[](const Host &host);
	};
} // namespace conf

#endif
