#ifndef HOST_PORT_HPP
#define HOST_PORT_HPP

#include "thin_string.hpp"

namespace http_abnf
{
	class HostPort
	{
	  private:
		ThinString host_;
		ThinString port_;

	  public:
		HostPort();
		HostPort(const ThinString &host, const ThinString &port);
		HostPort(const ThinString &str);
		HostPort(const HostPort &other);

		const ThinString &GetHost() const;
		const ThinString &GetPort() const;

		HostPort &operator=(const HostPort &other);
		bool      operator==(const HostPort &other) const;
		bool      operator!=(const HostPort &other) const;
	};
	std::ostream &operator<<(std::ostream &os, const HostPort &host_port);
} // namespace http_abnf

#endif
