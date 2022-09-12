#include "parse_uri_relative.hpp"

#include "parse_abnf_core_rules.hpp"
#include "parse_define.hpp"

namespace ABNF
{
	static const ThinString &kUnreservedUniqSet = "-._~";
	static const ThinString &kSubDelimsUniqSet  = "!$&'()*+,;=";

	// unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
	bool IsUnreserved(const char c)
	{
		return std::isalpha(c) || std::isdigit(c) || kUnreservedUniqSet.find(c) != ThinString::npos;
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
		return kSubDelimsUniqSet.find(c) != ThinString::npos;
	}
} // namespace ABNF
