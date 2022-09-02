#ifndef PARSE_PATH_HPP
#define PARSE_PATH_HPP

#include "ThinString.hpp"
#include "parse_define.hpp"

namespace ABNF
{
	bool IsPathAbempty(const ThinString &str);
	bool IsPathAbsolute(const ThinString &str);
	bool IsPathRootless(const ThinString &str);
	bool IsPathEmpty(const ThinString &str);

} // namespace ABNF

#endif
