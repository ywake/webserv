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

	const std::string &RequestMessage::GetMethod() const
	{
		return method_;
	}

	const RequestTarget &RequestMessage::GetRequestTarget() const
	{
		return request_target_;
	}

	const std::string &RequestMessage::GetHttpVersion() const
	{
		return http_version_;
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
