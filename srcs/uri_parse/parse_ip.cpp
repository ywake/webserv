#include "parse_ip.hpp"

#include "ThinString.hpp"
#include "parse_abnf_core_rules.hpp"
#include "parse_define.hpp"
#include "parse_ip_utils.hpp"
#include "result.hpp"
#include "webserv_utils.hpp"

namespace ABNF
{

	// IP-literal = "[" ( IPv6address / IPvFuture  ) "]"
	bool IsIPLiteral(const ThinString &str)
	{
		if (str.empty()) {
			return false;
		}
		if (str.at(0) == '[' && str.back() == ']') {
			ThinString parenthesis_trimmed = str.substr(1, str.len() - 2);
			return IsIPv6address(parenthesis_trimmed) || IsIPvFuture(parenthesis_trimmed);
		}
		return false;
	}

	// IPv6address   =                           6( h16 ":" ) ls32
	//              /                       "::" 5( h16 ":" ) ls32
	//              / [               h16 ] "::" 4( h16 ":" ) ls32
	//              / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
	//              / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
	//              / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
	//              / [ *4( h16 ":" ) h16 ] "::"              ls32
	//              / [ *5( h16 ":" ) h16 ] "::"              h16
	//              / [ *6( h16 ":" ) h16 ] "::"
	// TODO refactor
	bool IsIPv6address(const ThinString &str)
	{
		StringAry tokens = TokenizeIPv6address(str);
		static const std::size_t kNumOfTokenMax = 15;
		static const std::size_t kIpv6BytesMax = 16;
		if (tokens.empty()) {
			return false;
		}
		if (tokens.size() > kNumOfTokenMax) {
			return false;
		}
		if (HasMultiDcolon(tokens)) {
			return false;
		}
		Ipv6TokensPair left_right = DivideByDcolon(tokens);
		if (!IsValidColonPosition(left_right.first) || !IsValidColonPosition(left_right.second)) {
			return false;
		}
		Result<std::size_t> left_bytes = CountLeftBytes(left_right.first);
		if (left_bytes.IsErr()) {
			return false;
		}
		Result<std::size_t> right_bytes = CountRightBytes(left_right.second);
		if (right_bytes.IsErr()) {
			return false;
		}
		if (HasNoDcolon(tokens)) {
			return right_bytes.Val() == kIpv6BytesMax;
		}
		return left_bytes.Val() + right_bytes.Val() < kIpv6BytesMax;
	}

	// h16           = 1*4HEXDIG
	bool IsH16(const ThinString &str)
	{
		for (ThinString::const_iterator itr = str.begin(); itr != str.end(); itr++) {
			if (!IsHexDigit(*itr)) {
				return false;
			}
		}
		return str.size() >= 1 && str.size() <= 4;
	}

	// ls32          = ( h16 ":" h16 ) / IPv4address
	bool IsLS32(const ThinString &str)
	{
		ThinString::ThinStrPair pair = str.DivideBy(":");
		return (IsH16(pair.first) && IsH16(pair.second)) || IsIPv4address(str);
	}

	bool IsIPvFuture(const ThinString &str)
	{
		(void)str;
		return false;
	}

	// IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
	bool IsIPv4address(const ThinString &str)
	{
		StringAry array = Split(str, ".");
		static const std::size_t kNumOfDecOctet = 4;
		if (array.size() != kNumOfDecOctet) {
			return false;
		}
		for (StringAry::const_iterator it = array.begin(); it != array.end(); it++) {
			if (!IsDecOctet(*it)) {
				return false;
			}
		}
		return true;
	}

	// dec-octet = DIGIT                // 0-9
	//           / %x31-39 DIGIT         // 10-99
	//           / "1" 2DIGIT            // 100-199
	//           / "2" %x30-34 DIGIT     // 200-249
	//           / "25" %x30-35          // 250-255
	bool IsDecOctet(const ThinString &str)
	{
		if (str.empty()) {
			return false;
		}
		static const std::size_t max_len = sizeof("255") - 1;
		bool is_start_with_digit = std::isdigit(str.at(0));
		if (!is_start_with_digit || str.len() > max_len) {
			return false;
		}
		Result<long> res = StrToLong(str.ToString());
		return res.IsOk() && 0 <= res.Val() && res.Val() <= 255;
	}

	bool IsRegName(const ThinString &str)
	{
		(void)str;
		return false;
	}

} // namespace ABNF
