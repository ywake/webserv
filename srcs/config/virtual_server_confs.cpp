#include "virtual_server_confs.hpp"
#include <cassert>
namespace conf
{
	VirtualServerConfs::VirtualServerConfs(/* args */) : default_host_(), server_confs_() {}

	VirtualServerConfs::VirtualServerConfs(const Host &default_host, const HostMap &server_confs)
		: default_host_(default_host), server_confs_(server_confs)
	{}

	VirtualServerConfs::~VirtualServerConfs() {}

	bool VirtualServerConfs::operator==(const VirtualServerConfs &rhs) const
	{
		if (default_host_ != rhs.default_host_) {
			// std::cerr << "default_host_ is not equal" << std::endl;
			return false;
		}
		if (server_confs_.size() != rhs.server_confs_.size()) {
			// std::cerr << "server_confs_ size is not equal" << std::endl;
			return false;
		}
		for (HostMap::const_iterator it = server_confs_.begin(); it != server_confs_.end(); it++) {
			if (rhs.server_confs_.find(it->first) == rhs.server_confs_.end()) {
				// std::cerr << "server_confs_ key is not equal" << std::endl;
				return false;
			}
			if (*(it->second) != *(rhs.server_confs_.find(it->first)->second)) {
				// std::cerr << "server_confs_ value is not equal" << std::endl;
				return false;
			}
		}
		return true;
	}

	Result<const ServerConf &> VirtualServerConfs::operator[](const Host &host)
	{
		try {
			return *server_confs_[host];
		} catch (const std::out_of_range &e) {
		}

		try {
			return *server_confs_[default_host_.Value()];
		} catch (const std::out_of_range &e) {
			return Error("default_host is not set");
		}
	}

	void VirtualServerConfs::Add(const Host &host, ServerConf &server_conf)
	{
		if (default_host_.empty()) {
			default_host_ = host;
		}
		server_confs_.insert(std::pair<Host, ServerConf *>(host, &server_conf));
	}

	void VirtualServerConfs::Print(std::ostream &os) const
	{
		os << "default_host_: " << default_host_.Value() << std::endl;
		os << "server_confs_: " << std::endl;
		for (HostMap::const_iterator it = server_confs_.begin(); it != server_confs_.end(); it++) {
			os << "▽ ▽ " << it->first << "▽ ▽" << std::endl;
			os << *(it->second);
			os << "△ △ " << it->first << "△ △" << std::endl;
		}
	}

	std::ostream &operator<<(std::ostream &os, const VirtualServerConfs &virtual_server_confs)
	{
		virtual_server_confs.Print(os);
		return os;
	}
} // namespace conf