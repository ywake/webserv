#ifndef PARSE_HTTP_UTILS_HPP
#define PARSE_HTTP_UTILS_HPP

#include "thin_string.hpp"

namespace http
{
	ThinString              TrimOws(const ThinString &value);
	ThinString              TrimLeft(const ThinString &value);
	ThinString              TrimRight(const ThinString &value);
	std::vector<ThinString> ParseList(const ThinString &str, bool ignore_quote = true);

} // namespace http

#endif
