#ifndef REQUEST_MESSAGE_HPP
#define REQUEST_MESSAGE_HPP

#include "header_section.hpp"
#include "http_message.hpp"
#include "request_line.hpp"

namespace http
{
	class RequestMessage : public HttpMessage
	{
	  public:
		RequestLine request_line_;

		RequestMessage();
		RequestMessage(
			const RequestLine   &request_line,
			const HeaderSection &field_lines,
			const std::string   &message_body
		);
		bool operator==(const RequestMessage &rhs) const;
	};
} // namespace http
#endif
