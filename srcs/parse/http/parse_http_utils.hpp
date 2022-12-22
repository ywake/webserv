#ifndef PARSE_HTTP_UTILS_HPP
#define PARSE_HTTP_UTILS_HPP

#include <vector>

#include "result.hpp"
#include "thin_string.hpp"

namespace http
{
	ThinString              TrimOws(const ThinString &value);
	ThinString              TrimLeftOws(const ThinString &value);
	ThinString              TrimRightOws(const ThinString &value);
	std::vector<ThinString> ParseList(const ThinString &str, bool ignore_quote = true);
	Result<std::string>     NormalizePath(const ThinString &path);
} // namespace http

#endif
