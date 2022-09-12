#include "parse_ip.hpp"

#include "parse_abnf_core_rules.hpp"
#include "parse_define.hpp"
#include "parse_ip_utils.hpp"
#include "parse_path_utils.hpp"
#include "parse_uri_utils.hpp"
#include "result.hpp"
#include "thin_string.hpp"
#include "webserv_utils.hpp"

namespace ABNF
{
	static const size_t     kNumOfIpv6TokenMax = 15;
	static const size_t     kIpv6BytesMax      = 16;
	static const ThinString kIpvFutureUniqSet  = ":";

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
	bool IsIPv6address(const ThinString &str)
	{
		StringAry tokens = TokenizeIPv6address(str);
		if (tokens.empty() || tokens.size() > kNumOfIpv6TokenMax) {
			return false;
		}
		std::size_t num_of_dcolons = CountDcolons(tokens);
		if (num_of_dcolons > 1) {
			return false;
		}
		Result<std::size_t> bytes = CountIpv6Bytes(tokens);
		if (bytes.IsErr()) {
			return false;
		}
		bool has_omitted_bytes = num_of_dcolons == 1;
		if (has_omitted_bytes) {
			return bytes.Val() < kIpv6BytesMax;
		} else {
			return bytes.Val() == kIpv6BytesMax;
		}
	}

	// h16           = 1*4HEXDIG
	bool IsH16(const ThinString &str)
	{
		if (!IsHexDigitOnly(str)) {
			return false;
		}
		return str.size() >= 1 && str.size() <= 4;
	}

	// ls32          = ( h16 ":" h16 ) / IPv4address
	bool IsLS32(const ThinString &str)
	{
		ThinString::ThinStrPair pair = str.DivideBy(":");
		return (IsH16(pair.first) && IsH16(pair.second)) || IsIPv4address(str);
	}

	// IPvFuture     = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
	bool IsIPvFuture(const ThinString &str)
	{
		if (str.empty() || str.at(0) != 'v') {
			return false;
		}
		ThinString::ThinStrPair strs =
			str.DivideBy(".", ThinString::kAlignRight | ThinString::kKeepDelimRight);
		if (strs.second.empty() || strs.second.at(0) != '.') {
			return false;
		}
		ThinString trimed_v   = strs.first.substr(1);
		ThinString trimed_dot = strs.second.substr(1);
		return IsHexDigitOnly(trimed_v) && IsRegularUriCharOnly(trimed_dot, kIpvFutureUniqSet);
	}

	// IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
	bool IsIPv4address(const ThinString &str)
	{
		StringAry                array           = Split(str, ".");
		static const std::size_t kNumOfDecOctets = 4;
		if (array.size() != kNumOfDecOctets) {
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
		static const std::size_t max_len             = sizeof("255") - 1;
		bool                     is_start_with_digit = std::isdigit(str.at(0));
		if (!is_start_with_digit || str.len() > max_len) {
			return false;
		}
		Result<long> res = StrToLong(str.ToString());
		return res.IsOk() && 0 <= res.Val() && res.Val() <= 255;
	}

	// reg-name      = *( unreserved / pct-encoded / sub-delims )
	// TODO テスト追加
	bool IsRegName(const ThinString &str)
	{
		StringAry tokens = TokenizePchar(str);
		return IsRegularUriTokenOnly(tokens, "");
	}

} // namespace ABNF
