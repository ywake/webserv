#ifndef PARSE_AUTHORITY_HPP
#define PARSE_AUTHORITY_HPP

#include "thin_string.hpp"

namespace ABNF
{
	bool IsAuthority(const ThinString &str);
	bool IsUserInfo(const ThinString &str);
	bool IsHost(const ThinString &str);
	bool IsPort(const ThinString &str);

} // namespace ABNF

#endif
