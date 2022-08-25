#ifndef REQUEST_MESSAGE_HPP
#define REQUEST_MESSAGE_HPP

#include "http_message.hpp"
#include "request_line.hpp"

class RequestMessage : public HTTPMessage
{
  public:
	RequestLine request_line_;

	RequestMessage() : HTTPMessage(FieldLines(), "") {}

	RequestMessage(const std::string &str) : HTTPMessage(str)
	{
		// parse
	}

	RequestMessage(RequestLine request_line, FieldLines field_lines, std::string message_body)
		: HTTPMessage(field_lines, message_body), request_line_(request_line)
	{
	}

	bool operator==(const RequestMessage &rhs) const
	{
		return HTTPMessage::operator==(rhs) && request_line_ == rhs.request_line_;
	}
};

#endif
