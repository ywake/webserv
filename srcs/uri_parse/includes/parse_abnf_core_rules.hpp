#ifndef PARSE_ABNF_CORE_RULES_HPP
#define PARSE_ABNF_CORE_RULES_HPP
#include "thin_string.hpp"

namespace ABNF
{
	bool IsHexDigit(const char c);
	bool IsTchar(const char c);
	bool IsHexDigitOnly(const ThinString &str);
	bool IsTcharOnly(const ThinString &str);
} // namespace ABNF

#endif
