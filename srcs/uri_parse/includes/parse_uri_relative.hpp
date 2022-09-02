#ifndef PARSE_URI_RELATIVE_HPP
#define PARSE_URI_RELATIVE_HPP

#include "ThinString.hpp"

namespace ABNF
{
	bool IsUnreserved(const char c);
	bool IsPctEncoded(const ThinString &str);
	bool IsSubDelims(const char c);

} // namespace ABNF

#endif
