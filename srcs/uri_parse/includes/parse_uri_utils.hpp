#ifndef PARSE_URI_UTILS_HPP
#define PARSE_URI_UTILS_HPP

#include "ThinString.hpp"
#include "parse_define.hpp"

namespace ABNF
{
	bool IsRegularURIChar(const char c, const char *additional_char_set);
	bool IsRegularURICharAll(const ThinString &str, const char *additional_char_set);
	bool IsRegularURIToken(const ThinString &token, const char *additional_char_set);
	bool IsRegularURITokenAll(const StringAry &tokens, const char *additional_char_set);
	bool IsHexDigitAll(const ThinString &str);
} // namespace ABNF

#endif
