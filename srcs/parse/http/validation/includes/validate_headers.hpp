#ifndef VALIDATION_HEADERS_HPP
#define VALIDATION_HEADERS_HPP

#include "header_section.hpp"

namespace http
{
	namespace headers
	{
		void ValidateHeaderSection(const HeaderSection &field_lines);
		bool IsValidContentLength(const std::string &values);
		bool HasSingleHost(const HeaderSection::Values &values);
		bool IsImplementedTransferCoding(const HeaderSection::Values &values);
		bool IsValidTransferEncoding(const HeaderSection::Values &values);
	} // namespace headers
} // namespace http

#endif
