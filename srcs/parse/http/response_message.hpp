#ifndef RESPONSE_MESSAGE_HPP
#define RESPONSE_MESSAGE_HPP
#include "http_message.hpp"
#include "status_line.hpp"

namespace http
{
	class ResponseMessage : public HttpMessage
	{
	  public:
		StatusLine status_line_;

		ResponseMessage(
			StatusLine &status_line, HeaderSection &field_lines, std::string &message_body
		)
			: HttpMessage(field_lines, message_body), status_line_(status_line)
		{}

		bool operator==(const ResponseMessage &rhs) const
		{
			return HttpMessage::operator==(rhs) && status_line_ == rhs.status_line_;
		}
	};
} // namespace http

#endif
