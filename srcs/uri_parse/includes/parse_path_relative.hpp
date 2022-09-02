#ifndef PARSE_PATH_RELATIVE_HPP
#define PARSE_PATH_RELATIVE_HPP

#include "ThinString.hpp"
#include "parse_define.hpp"

namespace ABNF
{
	bool IsSegment(const ThinString &str);
	bool IsPchar(const ThinString &token);

} // namespace ABNF

#endif
