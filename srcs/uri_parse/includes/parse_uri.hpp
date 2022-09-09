#ifndef PARSE_URI_HPP
#define PARSE_URI_HPP

#include "ThinString.hpp"

namespace ABNF
{
	bool IsScheme(const ThinString &str);
	bool IsQuery(const ThinString &str);

} // namespace ABNF

#endif
