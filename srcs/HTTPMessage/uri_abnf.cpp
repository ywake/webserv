#include "uri_abnf.hpp"
#include <algorithm>
#include <cctype>
#include <cstring>

namespace ABNF
{
	static const char *kQueryUniqSet = "/?";
	static const char *kPcharUniqSet = ":@";
	static const char *kUserInfoUniqSet = ":";
	static const char *kUnreservedUniqSet = "-._~";
	static const char *kSubDelimsUniqSet = "!$&'()*+,;=";
	static const char *kSchemeUniqSet = "+-.";
	static const size_t kPctEncodingSize = 3;

	//[FIX]
	StringAry Split(const std::string &str, const std::string delim)
	{
		StringAry split;
		std::size_t delim_length = delim.length();
		if (delim_length == 0) {
			split.push_back(str);
			return split;
		}
		std::size_t offset = std::string::size_type(0);
		while (true) {
			std::size_t pos = str.find(delim, offset);
			if (pos == std::string::npos) {
				split.push_back(str.substr(offset));
				break;
			}
			split.push_back(str.substr(offset, pos - offset));
			offset = pos + delim_length;
		}
		return split;
	}

	StringAry TokenizePath(const ThinString &str)
	{
		StringAry tokens;
		ThinString head = str;

		while (1) {
			std::size_t slash_pos = head.MeasureUntil("/");
			if (slash_pos != 0) {
				tokens.push_back(head.substr(0, slash_pos)); // before '/'
			}
			bool is_end = slash_pos == head.len();
			if (is_end) {
				break;
			}
			tokens.push_back(head.substr(slash_pos, 1)); // "/"
			head = head.substr(slash_pos + 1);
		}

		return tokens;
	}

	bool IsValidPath(const ThinString &str)
	{
		StringAry tokens = TokenizePath(str);
		for (StringAry::const_iterator itr = tokens.begin(); itr != tokens.end(); itr++) {
			if (*itr == "/") {
				continue;
			}
			if (!IsSegment(*itr)) {
				return false;
			}
		}
		return true;
	}

	// path-rootless = segment-nz *( "/" segment )
	bool IsPathRootless(const ThinString &str)
	{
		if (str.empty() || str.at(0) == '/') {
			return false;
		}
		return IsValidPath(str);
	}

	// absolute-path = "/" [ segment-nz *( "/" segment ) ] -> start with "//"
	// absolute-path = 1 *( "/" segment ) -> start with "//" ok
	bool IsPathAbsolute(const ThinString &str)
	{
		if (str.empty() || str.at(0) != '/') {
			return false;
		}
		return IsValidPath(str);
	}

	bool IsPathAbempty(const ThinString &str)
	{
		if (str.empty()) {
			return true;
		}
		return IsPathAbsolute(str);
	}

	// query         = *( pchar / "/" / "?" )
	bool IsQuery(const ThinString &str)
	{
		StringAry token = TokenizePchar(str);
		for (StringAry::const_iterator itr = token.begin(); itr != token.end(); itr++) {
			bool is_valid_token = IsPchar(*itr) || std::strchr(kQueryUniqSet, itr->at(0));
			if (!is_valid_token) {
				return false;
			}
		}
		return true;
	}

	// segment       = *pchar
	bool IsSegment(const ThinString &str)
	{
		StringAry token = TokenizePchar(str);
		for (StringAry::const_iterator itr = token.begin(); itr != token.end(); itr++) {
			if (!IsPchar(*itr)) {
				return false;
			}
		}
		return true;
	}

	// abcd%88ab%99a
	// a, b, c, d, %88, a, b, %99, a
	StringAry TokenizePchar(const ThinString &str)
	{
		StringAry tokens;
		for (std::string::const_iterator itr = str.begin(); itr != str.end();) {
			std::size_t token_start = itr - str.begin();
			std::size_t token_len = *itr == '%' ? kPctEncodingSize : sizeof(char);
			tokens.push_back(str.substr(token_start, token_len));
			itr += tokens.back().size();
		}
		return tokens;
	}

	// unreserved / pct-encoded / sub-delims
	bool IsValidURIChar(const ThinString &token, const char *additional_char_set)
	{
		if (token.size() == kPctEncodingSize) {
			return IsPctEncoded(token);
		} else if (token.size() == sizeof(char)) {
			char c = *token.begin();
			return IsUnreserved(c) || IsSubDelims(c) || std::strchr(additional_char_set, c);
		} else {
			return false;
		}
	}

	// pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
	bool IsPchar(const ThinString &token)
	{
		return IsValidURIChar(token, kPcharUniqSet);
	}

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

	// HEXDIG
	bool IsHexDigit(const char c)
	{
		return std::isdigit(c) || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f');
	}

	bool IsScheme(const ThinString &str)
	{
		if (str.len() == 0 || !std::isalpha(str.at(0))) {
			return false;
		}
		for (ThinString::const_iterator it = str.begin(); it != str.end(); it++) {
			if (!std::isalnum(*it) && !std::strchr(kSchemeUniqSet, *it)) {
				return false;
			}
		}
		return true;
	}

	// hier-part    = "//" authority path-abempty
	//              / path-absolute
	//              / path-rootless ; Not support
	//              / path-empty
	bool IsHierPart(const ThinString &str)
	{
		if (IsPathEmpty(str)) {
			return true;
		} else if (str.find("//") == 0) {
			ThinString after_2slash = str.substr(2);
			ThinString::ThinStrPair authority_path =
				after_2slash.DivideBy("/", ThinString::kIncludeRight);
			return IsAuthority(authority_path.first) && IsPathAbempty(authority_path.second);
		} else if (IsPathAbsolute(str)) {
			return true;
		}
		return false;
	}

	// path-empty = 0<pchar>
	bool IsPathEmpty(const ThinString &str)
	{
		return str.empty();
	}

	// authority = [ userinfo "@" ] host [ ":" port ]
	bool IsAuthority(const ThinString &str)
	{
		ThinString::ThinStrPair userinfo_hostport_pair = str.DivideBy("@");
		ThinString userinfo = userinfo_hostport_pair.first;
		ThinString hostport = userinfo_hostport_pair.second;
		if (!IsUserInfo(userinfo)) {
			return false;
		}
		ThinString::ThinStrPair host_port_pair = hostport.DivideBy(":");
		ThinString host = host_port_pair.first;
		ThinString port = host_port_pair.second;
		return IsHost(host) && IsPort(port);
	}

	// userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
	bool IsUserInfo(const ThinString &str)
	{
		StringAry tokens = TokenizePchar(str);
		for (StringAry::const_iterator it = tokens.begin(); it != tokens.end(); it++) {
			if (!IsValidURIChar(*it, kUserInfoUniqSet)) {
				return false;
			}
		}
		return true;
	}

	// host = IP-literal / IPv4address / reg-name
	bool IsHost(const ThinString &str)
	{
		return IsIPLiteral(str) || IsIPv4address(str) || IsRegName(str);
	}

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

	bool IsIPv6address(const ThinString &str)
	{
		(void)str;
		return false;
	}

	bool IsIPvFuture(const ThinString &str)
	{
		(void)str;
		return false;
	}

	bool IsIPv4address(const ThinString &str)
	{
		(void)str;
		return false;
	}

	bool IsRegName(const ThinString &str)
	{
		(void)str;
		return false;
	}

	// port = *DIGIT
	bool IsPort(const ThinString &str)
	{
		(void)str;
		return false;
	}

} // namespace ABNF
