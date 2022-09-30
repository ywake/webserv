#include "parse_abnf_core_rules.hpp"
#include "thin_string.hpp"

namespace ABNF
{
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

	// VCHAR          =  %x21-7E
	bool IsVchar(char c)
	{
		return 0x21 <= c && c <= 0x7e;
	}

} // namespace ABNF
