#ifndef PARSE_URI_RELATIVE_HPP
#define PARSE_URI_RELATIVE_HPP

#include "thin_string.hpp"

namespace ABNF
{
	bool IsUnreserved(const char c);
	bool IsPctEncoded(const ThinString &str);
	bool IsSubDelims(const char c);

} // namespace ABNF

#endif
