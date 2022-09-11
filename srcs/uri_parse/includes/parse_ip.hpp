#ifndef PARSE_IP_HPP
#define PARSE_IP_HPP

#include "thin_string.hpp"

namespace ABNF
{
	bool IsIPLiteral(const ThinString &str);
	bool IsIPv6address(const ThinString &str);
	bool IsIPvFuture(const ThinString &str);
	bool IsH16(const ThinString &str);
	bool IsLS32(const ThinString &str);
	bool IsIPv4address(const ThinString &str);
	bool IsDecOctet(const ThinString &str);
	bool IsRegName(const ThinString &str);
} // namespace ABNF

#endif
