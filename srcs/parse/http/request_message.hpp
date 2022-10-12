#ifndef REQUEST_MESSAGE_HPP
#define REQUEST_MESSAGE_HPP

#include "http_message.hpp"
#include "request_line.hpp"

namespace http
{
	class RequestMessage : public HttpMessage
	{
	  public:
		RequestLine request_line_;

		RequestMessage();
		RequestMessage(const std::string &str);
		RequestMessage(
			RequestLine request_line, HeaderSection field_lines, std::string message_body
		);
		bool               operator==(const RequestMessage &rhs) const;
		const std::string &GetHost() const;
	};
} // namespace http

#endif
