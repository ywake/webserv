#include "virtual_server_confs.hpp"

namespace conf
{
	VirtualServerConfs::VirtualServerConfs(/* args */) {}

	VirtualServerConfs::~VirtualServerConfs() {}

	VirtualServerConf &VirtualServerConfs::operator[](const Host &host)
	{
		if (server_confs_.find(host) == server_confs_.end()) {
			return server_confs_[default_host_];
		}
		return server_confs_[host];
	}
} // namespace conf
