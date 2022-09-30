#ifndef PARSE_PATH_RELATIVE_HPP
#define PARSE_PATH_RELATIVE_HPP

#include "parse_define.hpp"
#include "thin_string.hpp"

namespace ABNF
{
	bool IsSegment(const ThinString &str);
	bool IsPchar(const ThinString &token);

} // namespace ABNF

#endif
