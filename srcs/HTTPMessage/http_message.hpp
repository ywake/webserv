#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP
#include "field_lines.hpp"
#include <string>

class HTTPMessage
{
  public:
	FieldLines field_lines_;
	std::string message_body_;

	HTTPMessage(FieldLines field_lines, std::string message_body)
		: field_lines_(field_lines), message_body_(message_body)
	{
	}

	HTTPMessage(std::string request)
	{
		(void)request;
		// TODO : parserは後で別関数に実装
	}

	bool operator==(const HTTPMessage &rhs) const
	{
		return field_lines_ == rhs.field_lines_ && message_body_ == rhs.message_body_;
	}
};

#endif
