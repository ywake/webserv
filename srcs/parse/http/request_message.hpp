#ifndef REQUEST_MESSAGE_HPP
#define REQUEST_MESSAGE_HPP

#include "header_section.hpp"
#include "http_message.hpp"
#include "request_line.hpp"

namespace http
{
	class RequestMessage : public HttpMessage
	{
	  private:
		RequestLine request_line_;

	  public:
		RequestMessage();
		RequestMessage(const RequestMessage &other);
		RequestMessage(
			const RequestLine   &request_line,
			const HeaderSection &field_lines,
			const std::string   &message_body
		);
		~RequestMessage();
		RequestMessage &operator=(const RequestMessage &rhs);
		bool            operator==(const RequestMessage &rhs) const;
	};
} // namespace http
#endif
