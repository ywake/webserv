#ifndef VALIDATE_REQUEST_LINE_HPP
#define VALIDATE_REQUEST_LINE_HPP

#include "thin_string.hpp"

namespace http
{
	namespace abnf
	{
		bool IsMethod(const ThinString &str);
		bool IsHttpVersion(const ThinString &str);
		bool IsVersion(const ThinString &str);
	} // namespace abnf
} // namespace http

#endif
