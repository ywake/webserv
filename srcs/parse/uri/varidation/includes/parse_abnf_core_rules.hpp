#ifndef PARSE_ABNF_CORE_RULES_HPP
#define PARSE_ABNF_CORE_RULES_HPP
#include "thin_string.hpp"

namespace ABNF
{
	bool IsDigitOnly(const ThinString &str);
	bool IsHexDigit(const char c);
	bool IsHexDigitOnly(const ThinString &str);
	bool IsVchar(char c);
} // namespace ABNF

#endif
