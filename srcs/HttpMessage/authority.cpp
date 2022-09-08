#include "authority.hpp"

#include "error.hpp"
#include "parse_authority.hpp"

Authority::Authority() : userinfo_(), host_(), port_() {}

Authority::Authority(const ThinString &authority) : userinfo_(), host_(), port_()
{
	ThinString::ThinStrPair userinfo_hostport = authority.DivideBy("@", ThinString::kAlignRight);
	ThinString hostport = userinfo_hostport.second;
	ThinString::ThinStrPair host_port = hostport.DivideBy(":");

	ThinString userinfo = userinfo_hostport.first;
	ThinString host = host_port.first;
	ThinString port = host_port.second;
	if (!ABNF::IsUserInfo(userinfo) || !ABNF::IsHost(host) || !ABNF::IsPort(port)) {
		throw Error("400");
	}
	userinfo_ = userinfo;
	host_ = host;
	port_ = port;
}

Authority &Authority::operator=(const Authority &other)
{
	if (this == &other) {
		return *this;
	}
	userinfo_ = other.userinfo_;
	host_ = other.host_;
	port_ = other.port_;
	return *this;
}
