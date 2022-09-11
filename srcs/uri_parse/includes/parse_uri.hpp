#ifndef PARSE_URI_HPP
#define PARSE_URI_HPP

#include "thin_string.hpp"

namespace ABNF
{
	bool IsScheme(const ThinString &str);
	bool IsQuery(const ThinString &str);

} // namespace ABNF

#endif
