#ifndef PARSE_URI_UTILS_HPP
#define PARSE_URI_UTILS_HPP

#include "parse_define.hpp"
#include "thin_string.hpp"

namespace ABNF
{
	bool IsRegularUriChar(const char c, const char *additional_char_set);
	bool IsRegularUriCharOnly(const ThinString &str, const char *additional_char_set);
	bool IsRegularUriToken(const ThinString &token, const char *additional_char_set);
	bool IsRegularUriTokenOnly(const StringAry &tokens, const char *additional_char_set);
} // namespace ABNF

#endif
