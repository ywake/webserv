#ifndef VALIDATION_HEADERS_HPP
#define VALIDATION_HEADERS_HPP

#include <field_lines.hpp>

namespace http_headers
{
	bool HasSingleHost(const FieldLines &field_lines);
	bool IsValidTransferEncoding(const FieldLines &field_lines);
	bool IsValidFieldLines(const FieldLines &field_lines);
	bool IsValidContentLength(const std::string &values);
} // namespace http_headers

#endif
