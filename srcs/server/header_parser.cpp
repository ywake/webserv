#include "header_parser.hpp"
#include "host_port.hpp"
#include "http_exceptions.hpp"
#include "validate_headers.hpp"

namespace server
{
	HeaderParser::HeaderParser() : FieldParser() {}

	HeaderParser::HeaderParser(const FieldParser &other) : FieldParser()
	{
		*this = other;
	}

	FieldParser &HeaderParser::operator=(const FieldParser &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		FieldParser::operator=(rhs);
		return *this;
	}

	Emptiable<http::FieldSection *> HeaderParser::Parse(q_buffer::QueuingBuffer &recieved)
	{
		Emptiable<http::FieldSection *> headers_or_empty = FieldParser::Parse(recieved);
		if (headers_or_empty.empty()) {
			return Emptiable<http::FieldSection *>();
		}
		http::FieldSection &headers = *headers_or_empty.Value();
		ValidateHost(headers["host"]);
		return &headers;
	}

	void HeaderParser::ValidateHost(const http::FieldSection::Values &values)
	{
		if (!http::headers::HasSingleHost(values)) {
			throw http::BadRequestException();
		}
		(void)http::abnf::HostPort(values.front().GetValue());
	}

} // namespace server
