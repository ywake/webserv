#ifndef PARSE_PATH_UTILS_HPP
#define PARSE_PATH_UTILS_HPP

#include "parse_define.hpp"
#include "thin_string.hpp"

namespace ABNF
{
	StringAry TokenizePath(const ThinString &str);
	StringAry TokenizePchar(const ThinString &str);

} // namespace ABNF

#endif
