#include "http_message.hpp"

namespace http
{
	HttpMessage::HttpMessage() : field_lines_(), message_body_() {}

	HttpMessage::HttpMessage(const HeaderSection &field_lines, const std::string &message_body)
		: field_lines_(field_lines), message_body_(message_body)
	{}

	bool HttpMessage::operator==(const HttpMessage &rhs) const
	{
		return field_lines_ == rhs.field_lines_ && message_body_ == rhs.message_body_;
	}
} // namespace http
