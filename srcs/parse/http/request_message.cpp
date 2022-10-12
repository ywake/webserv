#include "request_message.hpp"

RequestMessage::RequestMessage() : HttpMessage(HeaderSection(), "") {}

RequestMessage::RequestMessage(const std::string &str) : HttpMessage(str)
{
	// parse
}

RequestMessage::RequestMessage(
	RequestLine request_line, HeaderSection field_lines, std::string message_body
)
	: HttpMessage(field_lines, message_body), request_line_(request_line)
{}

bool RequestMessage::operator==(const RequestMessage &rhs) const
{
	return HttpMessage::operator==(rhs) && request_line_ == rhs.request_line_;
}
