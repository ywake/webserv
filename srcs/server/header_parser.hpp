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
	};
} // namespace server

#endif
