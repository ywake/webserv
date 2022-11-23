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
		HttpMessage(HeaderSection field_lines, std::string message_body);
		bool operator==(const HttpMessage &rhs) const;
	};
} // namespace http
#endif
