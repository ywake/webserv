#ifndef PARSE_HTTP_UTILS_HPP
#define PARSE_HTTP_UTILS_HPP

#include "thin_string.hpp"
#include <vector>

namespace http
{
	ThinString              TrimOws(const ThinString &value);
	ThinString              TrimLeftOws(const ThinString &value);
	ThinString              TrimRightOws(const ThinString &value);
	std::vector<ThinString> ParseList(const ThinString &str, bool ignore_quote = true);

} // namespace http

#endif
