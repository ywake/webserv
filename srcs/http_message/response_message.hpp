#ifndef RESPONSE_MESSAGE_HPP
#define RESPONSE_MESSAGE_HPP
#include "http_message.hpp"
#include "status_line.hpp"

class ResponseMessage : public HttpMessage
{
  public:
	StatusLine status_line_;

	ResponseMessage(StatusLine &status_line, FieldLines &field_lines, std::string &message_body)
		: HttpMessage(field_lines, message_body), status_line_(status_line)
	{}

	bool operator==(const ResponseMessage &rhs) const
	{
		return HttpMessage::operator==(rhs) && status_line_ == rhs.status_line_;
	}
};

#endif
