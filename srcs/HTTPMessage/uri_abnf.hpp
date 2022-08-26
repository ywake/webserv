#ifndef URI_ABNF_HPP
#define URI_ABNF_HPP

#include <string>
#include <vector>
#include "ThinString.hpp"

namespace ABNF
{
	typedef std::vector<ThinString> ThinStrAry;
	ThinStrAry Split(const std::string &str, const std::string delim);
	bool IsPathAbsolute(const ThinString &str);
	bool IsQuery(const ThinString  &str);
	bool IsSegment(const std::string &str);
	bool IsPchar(const std::string &str);
	bool IsUnreserved(const char c);
	bool IsPctEncoded(const std::string &str);
	bool IsSubDelims(const char c);
	bool IsHexDigit(const char c);
	std::vector<std::string> TokenizePchar(const std::string &str);
	ThinStrAry TokenizePathAbsolute(const std::string &str);
	std::size_t get_len_until(const std::string &src, const std::string &delim, std::size_t start);
} // namespace ABNF

#endif // URI_ABNF_HPP
