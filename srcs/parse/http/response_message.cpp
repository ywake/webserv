#include "response_message.hpp"

namespace http
{
	ResponseMessage::ResponseMessage(
		const StatusLine    &status_line,
		const HeaderSection &field_lines,
		const std::string   &message_body
	)
		: HttpMessage(field_lines, message_body), status_line_(status_line)
	{}

	bool ResponseMessage::operator==(const ResponseMessage &rhs) const
	{
		return HttpMessage::operator==(rhs) && status_line_ == rhs.status_line_;
	}
} // namespace http
