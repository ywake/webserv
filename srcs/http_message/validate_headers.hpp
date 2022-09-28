#ifndef VALIDATION_HEADERS_HPP
#define VALIDATION_HEADERS_HPP

#include <field_lines.hpp>

namespace http_headers
{
	bool HasSingleHost(const HeaderSection &field_lines);
	bool IsValidTransferEncoding(const HeaderSection &field_lines);
	bool IsValidFieldLines(const HeaderSection &field_lines);
	bool IsValidContentLength(const std::string &values);
} // namespace http_headers

#endif
