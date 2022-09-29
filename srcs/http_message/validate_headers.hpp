#ifndef VALIDATION_HEADERS_HPP
#define VALIDATION_HEADERS_HPP

#include "header_section.hpp"

namespace http_headers
{
	bool HasSingleHost(const HeaderSection &field_lines);
	bool IsValidTransferEncoding(const HeaderSection &field_lines);
	bool IsValidFieldLines(const HeaderSection &field_lines);
	bool IsValidContentLength(const std::string &values);
} // namespace http_headers

#endif
