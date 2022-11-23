#ifndef RESPONSE_MESSAGE_HPP
#define RESPONSE_MESSAGE_HPP
#include "http_message.hpp"
#include "status_line.hpp"

namespace http
{
	class ResponseMessage : public HttpMessage
	{
	  private:
		StatusLine status_line_;

	  public:
		ResponseMessage(
			const StatusLine    &status_line,
			const HeaderSection &field_lines,
			const std::string   &message_body
		);

		bool operator==(const ResponseMessage &rhs) const;
	};
} // namespace http
#endif
