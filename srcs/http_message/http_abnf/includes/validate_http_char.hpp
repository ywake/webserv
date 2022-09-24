#ifndef VALIDATE_HTTP_CHAR_HPP
#define VALIDATE_HTTP_CHAR_HPP

#include "thin_string.hpp"

namespace http_abnf
{
	bool IsTchar(const char c);
	bool IsToken(const ThinString &str);
} // namespace http_abnf

#endif
