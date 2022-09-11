#include "parse_uri_relative.hpp"

#include "parse_abnf_core_rules.hpp"
#include "parse_define.hpp"
#include <cstring>

namespace ABNF
{
	static const char *kUnreservedUniqSet = "-._~";
	static const char *kSubDelimsUniqSet  = "!$&'()*+,;=";

	// unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
	bool IsUnreserved(const char c)
	{
		return std::isalpha(c) || std::isdigit(c) || std::strchr(kUnreservedUniqSet, c);
	}

	// pct-encoded   = "%" HEXDIG HEXDIG
	bool IsPctEncoded(const ThinString &str)
	{
		return str.size() == kPctEncodingSize && str.at(0) == '%' && IsHexDigit(str.at(1)) &&
			   IsHexDigit(str.at(2));
	}

	// sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
	//              / "*" / "+" / "," / ";" / "="
	bool IsSubDelims(const char c)
	{
		return std::strchr(kSubDelimsUniqSet, c);
	}
} // namespace ABNF
