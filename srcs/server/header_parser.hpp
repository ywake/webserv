#ifndef HEADER_PARSER_HPP
#define HEADER_PARSER_HPP

#include "field_parser.hpp"

namespace server
{
	class HeaderParser : public FieldParser
	{
	  public:
		HeaderParser();
		HeaderParser(const FieldParser &other);
		FieldParser                    &operator=(const FieldParser &rhs);
		Emptiable<http::FieldSection *> Parse(q_buffer::QueuingBuffer &recieved);

	  private:
		void                       ParseEachHeaders(http::FieldSection &headers);
		void                       ValidateHost(const http::FieldSection::Values &values);
		void                       ValidateContentLength(const http::FieldSection::Values &values);
		http::FieldSection::Values ParseConnection(const http::FieldSection::Values &values);
		http::FieldSection::Values ParseTransferEncoding(const http::FieldSection::Values &values);
	};
} // namespace server

#endif
