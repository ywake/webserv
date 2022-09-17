#ifndef VALIDATE_SPACE_HPP
#define VALIDATE_SPACE_HPP
#include "thin_string.hpp"

namespace http_abnf
{
	bool IsOws(const ThinString &str);
	bool IsRws(const ThinString &str);
} // namespace http_abnf

#endif
