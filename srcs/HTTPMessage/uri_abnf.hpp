#ifndef URI_ABNF_HPP
#define URI_ABNF_HPP

#include <string>
#include <vector>

namespace ABNF
{
	typedef std::vector<std::string> StringAry;
	StringAry Split(const std::string &str, const std::string delim);
	bool IsPathAbsolute(const std::string &str);
	bool IsQuery(const std::string &str);
	bool IsSegment(const std::string &str);
	bool IsPchar(const std::string &str);
	bool IsUnreserved(const char c);
	bool IsPctEncoded(const std::string &str);
	bool IsSubDelims(const char c);
	bool IsHexDigit(const char c);
	std::vector<std::string> TokenizePchar(const std::string &str);
	StringAry TokenizePathAbsolute(const std::string &str);
	std::size_t get_len_until(const std::string &src, const std::string &delim, std::size_t start);
} // namespace ABNF

#endif // URI_ABNF_HPP
