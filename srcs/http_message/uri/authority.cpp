#include "authority.hpp"

#include "http_exceptions.hpp"
#include "parse_authority.hpp"

Authority::Authority() : userinfo_(), host_(), port_() {}

// authority = [ userinfo "@" ] host [ ":" port ]
Authority::Authority(const ThinString &authority) : userinfo_(), host_(), port_()
{
	ThinString::ThinStrPair userinfo_hostport = authority.DivideBy("@", ThinString::kAlignRight);
	ThinString              hostport          = userinfo_hostport.second;
	ThinString::ThinStrPair host_port         = hostport.DivideBy(":");

	ThinString userinfo = userinfo_hostport.first;
	ThinString host     = host_port.first;
	ThinString port     = host_port.second;
	if (!ABNF::IsUserInfo(userinfo) || !ABNF::IsHost(host) || !ABNF::IsPort(port)) {
		throw BadRequestException();
	}
	userinfo_ = userinfo;
	host_     = host;
	port_     = port;
}

Authority::Authority(const ThinString &userinfo, const ThinString &host, const ThinString &port)
	: userinfo_(userinfo), host_(host), port_(port)
{}

Authority::Authority(const Authority &other)
	: userinfo_(other.userinfo_), host_(other.host_), port_(other.port_)
{}

Authority &Authority::operator=(const Authority &other)
{
	if (this == &other) {
		return *this;
	}
	userinfo_ = other.userinfo_;
	host_     = other.host_;
	port_     = other.port_;
	return *this;
}

const ThinString &Authority::GetUserinfo() const
{
	return userinfo_;
}

const ThinString &Authority::GetHost() const
{
	return host_;
}

const ThinString &Authority::GetPort() const
{
	return port_;
}

bool Authority::operator==(const Authority &other) const
{
	if (this == &other) {
		return true;
	}
	return userinfo_ == other.userinfo_ && host_ == other.host_ && port_ == other.port_;
}

bool Authority::operator!=(const Authority &other) const
{
	return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Authority &authority)
{
	os << "userinfo:" << authority.GetUserinfo() << std::endl;
	os << "host    :" << authority.GetHost() << std::endl;
	os << "port    :" << authority.GetPort() << std::endl;
	return os;
}
