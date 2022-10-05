#ifndef PARSE_IP_UTILS_HPP
#define PARSE_IP_UTILS_HPP

#include "parse_define.hpp"
#include "result.hpp"
#include "thin_string.hpp"

namespace ABNF
{
	std::size_t         CountDcolons(const StringAry &tokens);
	bool                IsValidColonPosition(StringAry &tokens);
	Ipv6TokensPair      DivideByDcolon(const StringAry &tokens);
	Result<std::size_t> CountLeftBytes(const StringAry &tokens);
	Result<std::size_t> CountRightBytes(const StringAry &tokens);
	StringAry           TokenizeIPv6address(const ThinString &str);
	Result<std::size_t> CountIpv6Bytes(const StringAry &tokens);

} // namespace ABNF

#endif
