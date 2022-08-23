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

	// absolute-path
	//	 = 1*( "/" segment )
	bool IsPathAbsolute(const std::string &str)
	{
		if (str.empty() || str.at(0) != '/') {
			return false;
		}
		StringAry segments = Split(str, "/");
		for (StringAry::const_iterator itr = segments.begin(); itr != segments.end(); itr++) {
			if (!IsSegment(*itr)) {
				return false;
			}
		}
		return true;
	}

	// query         = *( pchar / "/" / "?" )
	bool IsQuery(const std::string &str)
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
	bool IsSegment(const std::string &str)
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
	StringAry TokenizePchar(const std::string &str)
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
	bool IsPchar(const std::string &token)
	{
		if (token.size() == kPctEncodingSize) {
			return IsPctEncoded(token);
		} else if (token.size() == sizeof(char)) {
			char c = token.at(0);
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
	bool IsPctEncoded(const std::string &str)
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
} // namespace ABNF
