#ifndef URI_ABNF_HPP
#define URI_ABNF_HPP

#include "ThinString.hpp"
#include <string>
#include <vector>

namespace ABNF
{
	typedef std::vector<ThinString> StringAry;
	typedef std::pair<StringAry, StringAry> Ipv6TokensPair;

	StringAry Split(const ThinString &str, const std::string delim);
	bool IsPathAbsolute(const ThinString &str);
	bool IsQuery(const ThinString &str);
	bool IsSegment(const ThinString &str);
	bool IsValidURIChar(const ThinString &token, const char *additional_char_set);
	bool IsPchar(const ThinString &str);
	bool IsUnreserved(const char c);
	bool IsPctEncoded(const ThinString &str);
	bool IsSubDelims(const char c);
	bool IsHexDigit(const char c);
	bool IsScheme(const ThinString &str);
	bool IsHierPart(const ThinString &str);
	bool IsPathEmpty(const ThinString &str);
	bool IsPathRootless(const ThinString &str);
	bool IsPathAbempty(const ThinString &str);
	bool IsAuthority(const ThinString &str);
	bool IsUserInfo(const ThinString &str);
	bool IsHost(const ThinString &str);
	bool IsIPLiteral(const ThinString &str);
	bool IsIPv6address(const ThinString &str);
	bool IsIPvFuture(const ThinString &str);
	bool IsIPv4address(const ThinString &str);
	bool IsDecOctet(const ThinString &str);
	bool IsRegName(const ThinString &str);
	bool IsPort(const ThinString &str);
	bool IsValidColonPosition(StringAry &tokens);
	bool IsH16(const ThinString &str);
	StringAry TokenizePchar(const ThinString &str);
	StringAry TokenizePath(const ThinString &str);
	StringAry TokenizeIPv6address(const ThinString &str);
	Ipv6TokensPair DivideByDcolon(const StringAry &tokens);
} // namespace ABNF

#endif // URI_ABNF_HPP
