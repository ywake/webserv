#include "parse_uri_relative.hpp"

#include "parse_abnf_core_rules.hpp"
#include "parse_define.hpp"
#include <cstring>

// const char *にするとsizeofで長さが取れないのでdefineする
#define UNRESERVED_UNIQ_SET "-._~"
#define SUB_DELIMS_UNIQ_SET "!$&'()*+,;="

namespace ABNF
{

	// unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
	bool IsUnreserved(const char c)
	{
		return std::isalpha(c) || std::isdigit(c) ||
			   std::memchr(UNRESERVED_UNIQ_SET, c, sizeof(UNRESERVED_UNIQ_SET) - 1);
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
		return std::memchr(SUB_DELIMS_UNIQ_SET, c, sizeof(SUB_DELIMS_UNIQ_SET) - 1);
	}
} // namespace ABNF
