#include "parse_abnf_core_rules.hpp"

#include <cctype>

namespace ABNF
{
	// HEXDIG
	bool IsHexDigit(const char c)
	{
		return std::isdigit(c) || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f');
	}
} // namespace ABNF
