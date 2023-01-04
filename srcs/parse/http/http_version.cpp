#include "http_version.hpp"
#include "validate_request_line.hpp"

namespace http
{
	// HTTP-version = "HTTP/" DIGIT "." DIGIT
	Result<HttpVersion> ParseHtttpVersion(const std::string &s)
	{
		static const std::string http      = "HTTP";
		static const std::size_t kMajorIdx = (http + "/").size();

		if (!http::abnf::IsHttpVersion(s)) {
			return Error();
		}
		unsigned int major = s[kMajorIdx] - '0';
		unsigned int minor = *s.rbegin() - '0';
		HttpVersion  v     = {http, major, minor};
		return v;
	}
} // namespace http
