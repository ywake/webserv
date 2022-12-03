#include "http_message.hpp"

namespace http
{
	HttpMessage::HttpMessage() : field_lines_(), message_body_() {}

	HttpMessage::HttpMessage(const HttpMessage &other)
	{
		*this = other;
	}

	HttpMessage::HttpMessage(const HeaderSection &field_lines, const std::string &message_body)
		: field_lines_(field_lines), message_body_(message_body)
	{}

	const HeaderSection &HttpMessage::GetHeaderSection() const
	{
		return field_lines_;
	}

	void HttpMessage::SetHeaderSection(const HeaderSection &field_lines)
	{
		field_lines_ = field_lines;
	}

	void HttpMessage::SetBody(const std::string &body)
	{
		message_body_ = body;
	}

	bool HttpMessage::HasMessageBody() const
	{
		return field_lines_.Contains("content-length") ||
			   field_lines_.Contains("transfer-encoding");
	}

	HttpMessage &HttpMessage::operator=(const HttpMessage &rhs)
	{
		if (this == &rhs) {
			return *this;
		}
		field_lines_  = rhs.field_lines_;
		message_body_ = rhs.message_body_;
		return *this;
	}

	bool HttpMessage::operator==(const HttpMessage &rhs) const
	{
		return field_lines_ == rhs.field_lines_ && message_body_ == rhs.message_body_;
	}

	HttpMessage::~HttpMessage() {}

} // namespace http
