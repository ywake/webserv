#ifndef HTTP_VERSION_HPP
#define HTTP_VERSION_HPP

#include "result.hpp"

namespace http
{
	typedef struct HttpVersion {
		std::string  name;
		unsigned int major;
		unsigned int minor;
	} HttpVersion;

	Result<HttpVersion> ParseHtttpVersion(const std::string &s);
} // namespace http
#endif
