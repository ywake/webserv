#include "request_message.hpp"

namespace http
{
	RequestMessage::RequestMessage() : HttpMessage(), request_line_() {}

	RequestMessage::RequestMessage(const RequestMessage &other) : HttpMessage(other)
	{
		*this = other;
	}

	RequestMessage::RequestMessage(
		const RequestLine   &request_line,
		const HeaderSection &field_lines,
		const std::string   &message_body
	)
		: HttpMessage(field_lines, message_body), request_line_(request_line)
	{}

	void RequestMessage::SetRequestLine(const RequestLine &request_line)
	{
		request_line_ = request_line;
	}

	RequestMessage &RequestMessage::operator=(const RequestMessage &rhs)
	{
		if (this == &rhs) {
			return *this;
		}
		request_line_ = rhs.request_line_;
		HttpMessage::operator=(rhs);
		return *this;
	}

	bool RequestMessage::operator==(const RequestMessage &rhs) const
	{
		return HttpMessage::operator==(rhs) && request_line_ == rhs.request_line_;
	}

	RequestMessage::~RequestMessage() {}

} // namespace http
