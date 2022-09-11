#ifndef PARSE_PATH_HPP
#define PARSE_PATH_HPP

#include "parse_define.hpp"
#include "thin_string.hpp"

namespace ABNF
{
	bool IsPathAbempty(const ThinString &str);
	bool IsPathAbsolute(const ThinString &str);
	bool IsPathRootless(const ThinString &str);
	bool IsPathEmpty(const ThinString &str);

} // namespace ABNF

#endif
