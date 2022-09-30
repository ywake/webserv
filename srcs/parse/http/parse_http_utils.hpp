#ifndef PARSE_HTTP_UTILS_HPP
#define PARSE_HTTP_UTILS_HPP

#include "thin_string.hpp"

namespace http
{
	ThinString TrimOws(const ThinString &value);

} // namespace http

#endif
