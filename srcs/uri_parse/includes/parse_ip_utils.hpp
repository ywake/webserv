#ifndef PARSE_IP_UTILS_HPP
#define PARSE_IP_UTILS_HPP

#include "ThinString.hpp"
#include "parse_define.hpp"
#include "result.hpp"

namespace ABNF
{
	bool HasMultiDcolon(const StringAry &tokens);
	bool HasNoDcolon(const StringAry &tokens);
	bool IsValidColonPosition(StringAry &tokens);
	Ipv6TokensPair DivideByDcolon(const StringAry &tokens);
	Result<std::size_t> CountLeftBytes(const StringAry &tokens);
	Result<std::size_t> CountRightBytes(const StringAry &tokens);
	StringAry TokenizeIPv6address(const ThinString &str);

} // namespace ABNF

#endif
