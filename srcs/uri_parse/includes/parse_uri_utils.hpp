#ifndef PARSE_URI_UTILS_HPP
#define PARSE_URI_UTILS_HPP

#include "ThinString.hpp"

namespace ABNF
{
	bool IsValidURIChar(const ThinString &token, const char *additional_char_set);
	bool IsHexDigitAll(const ThinString &str);

} // namespace ABNF

#endif
