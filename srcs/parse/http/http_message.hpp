#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP

#include <string>
#include <vector>

#include "header_section.hpp"

namespace http
{
	class HttpMessage
	{
	  private:
		HeaderSection field_lines_;
		std::string   message_body_;

	  public:
		HttpMessage(HeaderSection field_lines, std::string message_body)
			: field_lines_(field_lines), message_body_(message_body)
		{}

		HttpMessage(std::string request)
		{
			(void)request;
			// TODO : parserは後で別関数に実装
		}

		bool operator==(const HttpMessage &rhs) const
		{
			return field_lines_ == rhs.field_lines_ && message_body_ == rhs.message_body_;
		}
	};
} // namespace http
#endif
