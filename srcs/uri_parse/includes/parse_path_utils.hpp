#ifndef PARSE_PATH_UTILS_HPP
#define PARSE_PATH_UTILS_HPP

#include "ThinString.hpp"
#include "parse_define.hpp"

namespace ABNF
{
	StringAry TokenizePath(const ThinString &str);
	StringAry TokenizePchar(const ThinString &str);

} // namespace ABNF

#endif
