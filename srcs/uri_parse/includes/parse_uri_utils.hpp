#ifndef PARSE_URI_UTILS_HPP
#define PARSE_URI_UTILS_HPP

#include "parse_define.hpp"
#include "thin_string.hpp"

namespace ABNF
{
	bool IsRegularUriChar(const char c, const ThinString &additional_char_set);
	bool IsRegularUriCharOnly(const ThinString &str, const ThinString &additional_char_set);
	bool IsRegularUriToken(const ThinString &token, const ThinString &additional_char_set);
	bool IsRegularUriTokenOnly(const StringAry &tokens, const ThinString &additional_char_set);
} // namespace ABNF

#endif
