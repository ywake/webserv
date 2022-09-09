#ifndef REQUEST_MESSAGE_HPP
#define REQUEST_MESSAGE_HPP

#include "http_message.hpp"
#include "request_line.hpp"

class RequestMessage : public HttpMessage
{
  public:
	RequestLine request_line_;

	RequestMessage() : HttpMessage(FieldLines(), "") {}

	RequestMessage(const std::string &str) : HttpMessage(str)
	{
		// parse
	}

	RequestMessage(RequestLine request_line, FieldLines field_lines, std::string message_body)
		: HttpMessage(field_lines, message_body), request_line_(request_line)
	{}

	bool operator==(const RequestMessage &rhs) const
	{
		return HttpMessage::operator==(rhs) && request_line_ == rhs.request_line_;
	}
};

#endif
