#ifndef PARSE_AUTHORITY_HPP
#define PARSE_AUTHORITY_HPP

#include "ThinString.hpp"

namespace ABNF
{
	bool IsAuthority(const ThinString &str);
	bool IsUserInfo(const ThinString &str);
	bool IsHost(const ThinString &str);
	bool IsPort(const ThinString &str);

} // namespace ABNF

#endif
