#include "host_port.hpp"
#include "http_exceptions.hpp"
#include "parse_authority.hpp"

namespace http_abnf
{
	HostPort::HostPort() : host_(), port_() {}

	HostPort::HostPort(const ThinString &host, const ThinString &port) : host_(host), port_(port) {}

	// hostport = host [ ":" port ]
	HostPort::HostPort(const ThinString &str)
	{
		ThinString::ThinStrPair host_port = str.DivideBy(":");

		ThinString host = host_port.first;
		ThinString port = host_port.second;
		if (!ABNF::IsHost(host) || !ABNF::IsPort(port)) {
			throw BadRequestException();
		}
		host_ = host;
		port_ = port;
	}

	HostPort::HostPort(const HostPort &other) : host_(other.host_), port_(other.port_) {}

	const ThinString &HostPort::GetHost() const
	{
		return host_;
	}

	const ThinString &HostPort::GetPort() const
	{
		return port_;
	}

	HostPort &HostPort::operator=(const HostPort &other)
	{
		if (this == &other) {
			return *this;
		}
		host_ = other.host_;
		port_ = other.port_;
		return *this;
	}

	bool HostPort::operator==(const HostPort &other) const
	{
		if (this == &other) {
			return true;
		}
		return host_ == other.host_ && port_ == other.port_;
	}

	bool HostPort::operator!=(const HostPort &other) const
	{
		return !(*this == other);
	}

} // namespace http_abnf

std::ostream &operator<<(std::ostream &os, const http_abnf::HostPort &host_port)
{
	os << "host    :" << host_port.GetHost() << std::endl;
	os << "port    :" << host_port.GetPort() << std::endl;
	return os;
}
