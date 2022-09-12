#include "parse_abnf_core_rules.hpp"
#include "thin_string.hpp"

#include <cctype>

namespace ABNF
{
	static const ThinString kTcharUniqSet = "!#$%&'*+-.^_`|~";

	// HEXDIG
	bool IsHexDigit(const char c)
	{
		return std::isdigit(c) || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f');
	}

	bool IsHexDigitOnly(const ThinString &str)
	{
		if (str.empty()) {
			return false;
		}
		for (ThinString::const_iterator itr = str.begin(); itr != str.end(); itr++) {
			if (!IsHexDigit(*itr)) {
				return false;
			}
		}
		return true;
	}

	bool IsTchar(const char c)
	{
		return std::isdigit(c) || std::isalpha(c) || kTcharUniqSet.find(c) != ThinString::npos;
	}

	bool IsTcharOnly(const ThinString &str)
	{
		if (str.empty()) {
			return false;
		}
		for (ThinString::const_iterator itr = str.begin(); itr != str.end(); itr++) {
			if (!IsTchar(*itr)) {
				return false;
			}
		}
		return true;
	}
} // namespace ABNF
