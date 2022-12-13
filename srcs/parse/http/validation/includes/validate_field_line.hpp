#ifndef VALIDATE_FIELD_LINE_HPP
#define VALIDATE_FIELD_LINE_HPP
#include "thin_string.hpp"

namespace http
{
	namespace abnf
	{
		bool IsOws(const ThinString &str);
		bool IsRws(const ThinString &str);
		bool IsRws(char c);
		bool StartWithObsFold(const ThinString &str);
		bool EndWithObsFold(const ThinString &str);
		bool IsFieldName(const ThinString &str);
		bool IsFieldValue(const ThinString &str);
		bool IsFieldVchar(unsigned char c);
		bool IsObsText(unsigned char c);
	} // namespace abnf
} // namespace http

#endif
