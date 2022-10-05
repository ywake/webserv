#include "validate_request_line.hpp"
#include "validate_http_char.hpp"

namespace http
{
	namespace abnf
	{
		bool IsMethod(const ThinString &str)
		{
			return IsToken(str);
		}

		// HTTP-version = HTTP-name "/" DIGIT "." DIGIT
		bool IsHttpVersion(const ThinString &str)
		{
			ThinString::ThinStrPair http_version_pair =
				str.DivideBy("/", ThinString::kKeepDelimLeft);
			ThinString http    = http_version_pair.first;
			ThinString version = http_version_pair.second;
			return http == "HTTP/" && IsVersion(version);
		}

		// DIGIT "." DIGIT
		bool IsVersion(const ThinString &str)
		{
			static const std::size_t kVersionLen = 3;
			static const std::size_t kMajorIdx   = 0;
			static const std::size_t kDotIdx     = 1;
			static const std::size_t kMinorIdx   = 2;
			return str.size() == kVersionLen && std::isdigit(str.at(kMajorIdx)) &&
				   str.at(kDotIdx) == '.' && std::isdigit(str.at(kMinorIdx));
		}

	} // namespace abnf
} // namespace http
