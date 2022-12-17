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
	extern const std::string kConnection;
	extern const std::string kContentLength;
	extern const std::string kTransferEncoding;
	extern const std::string kChunked;
	extern const std::string kHost;
	extern const std::string kServerName;

	namespace methods
	{
		extern const std::string kGet;
		extern const std::string kDelete;
		extern const std::string kPost;
	} // namespace methods
} // namespace http

#endif
