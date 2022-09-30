#include <cctype>

#include "validate_http_char.hpp"

static const ThinString kTcharUniqSet = "!#$%&'*+-.^_`|~";

namespace http
{
	namespace abnf
	{
		bool IsTchar(const char c)
		{
			return std::isdigit(c) || std::isalpha(c) || kTcharUniqSet.find(c) != ThinString::npos;
		}

		// token = 1*tchar
		bool IsToken(const ThinString &str)
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
	} // namespace abnf
} // namespace http
