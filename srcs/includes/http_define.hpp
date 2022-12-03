#ifndef HTTP_DEFINE_HPP
#define HTTP_DEFINE_HPP

#include <string>

namespace http
{
	extern const std::string kCrLf;
	extern const std::string kEmptyLine;
	extern const std::string kWhiteSpaces;
	extern const std::string kSp;
	extern const std::string kHttpVersion;

	namespace methods
	{
		extern const std::string kGet;
		extern const std::string kDelete;
		extern const std::string kPost;
	} // namespace methods
} // namespace http

#endif
