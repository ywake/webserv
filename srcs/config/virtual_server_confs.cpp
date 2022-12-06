#include "virtual_server_confs.hpp"
#include <cassert>
namespace conf
{
	VirtualServerConfs::VirtualServerConfs(/* args */) : default_host_(), server_confs_() {}

	VirtualServerConfs::VirtualServerConfs(const VirtualServerConfs &other)
	{
		*this = other;
	}

	VirtualServerConfs::VirtualServerConfs(const Host &default_host)
		: default_host_(default_host), server_confs_()
	{}

	VirtualServerConfs::VirtualServerConfs(const Host &default_host, const HostMap &server_confs)
		: default_host_(default_host), server_confs_(server_confs)
	{}

	VirtualServerConfs::~VirtualServerConfs() {}

	VirtualServerConfs &VirtualServerConfs::operator=(const VirtualServerConfs &other)
	{
		if (this == &other) {
			return *this;
		}
		default_host_ = other.default_host_;
		server_confs_ = other.server_confs_;
		return *this;
	}

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

	const ServerConf &VirtualServerConfs::operator[](const Host &host) const
	{
		try {
			return *server_confs_.at(host);
		} catch (const std::out_of_range &e) {
			return *server_confs_.at(default_host_); // 例外は出ないはず
		}
	}

	const ServerConf &VirtualServerConfs::GetDefaultServerConf() const
	{
		return *server_confs_.at(default_host_); // 例外は出ないはず
	}

	void VirtualServerConfs::Add(const Host &host, const ServerConf &server_conf)
	{
		server_confs_.insert(std::pair<Host, const ServerConf *>(host, &server_conf));
	}

	void VirtualServerConfs::Print(std::ostream &os) const
	{
		os << "default_host_: " << default_host_ << std::endl;
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
