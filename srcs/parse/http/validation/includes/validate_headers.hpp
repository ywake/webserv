#ifndef VALIDATION_HEADERS_HPP
#define VALIDATION_HEADERS_HPP

#include "header_section.hpp"

namespace http
{
	namespace headers
	{
		void ValidateHeaderSection(const HeaderSection &field_lines);
		bool HasSingleHost(const HeaderSection &field_lines);
		bool IsValidTransferEncoding(const HeaderSection &field_lines);
		bool IsValidContentLength(const std::string &values);
	} // namespace headers
} // namespace http

#endif
