#include "parse_abnf_core_rules.hpp"
#include "thin_string.hpp"

#include <cctype>
#include <cstring>

// const char *にするとsizeofで長さが取れないのでdefineする
#define TCHAR_UNIQ_SET "!#$%&'*+-.^_`|~"

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

	bool IsTchar(const char c)
	{
		return std::isdigit(c) || std::isalpha(c) ||
			   std::memchr(TCHAR_UNIQ_SET, c, sizeof(TCHAR_UNIQ_SET) - 1);
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
