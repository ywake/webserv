#include "uri_abnf.hpp"
#include <algorithm>
#include <cctype>
#include <cstring>

namespace ABNF
{
	static const char *kQueryUniqSet = "/?";
	static const char *kPcharUniqSet = ":@";
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

	StringAry TokenizePathAbsolute(const ThinString &str)
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

	// absolute-path
	//	 = 1*( "/" segment )
	bool IsPathAbsolute(const ThinString &str)
	{
		if (str.empty() || str.at(0) != '/') {
			return false;
		}
		StringAry tokens = TokenizePathAbsolute(str);
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

	// pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
	bool IsPchar(const ThinString &token)
	{
		if (token.size() == kPctEncodingSize) {
			return IsPctEncoded(token);
		} else if (token.size() == sizeof(char)) {
			char c = *token.begin();
			return IsUnreserved(c) || IsSubDelims(c) || std::strchr(kPcharUniqSet, c);
		} else {
			return false;
		}
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

	bool IsHeirPart(const ThinString &str)
	{
		(void)str;
		return true;
	}
} // namespace ABNF
