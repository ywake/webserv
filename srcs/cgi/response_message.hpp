#ifndef RESPONSE_MESSAGE_HPP
#define RESPONSE_MESSAGE_HPP

#include "header_value.hpp"
#include "http_define.hpp"

namespace cgi
{
	class ResponseMessage
	{
	  public:
		typedef std::list<HeaderValue>              Values;
		typedef std::map<const std::string, Values> Headers;

		ResponseMessage();

	  private:
		// HeaderFields headers_;
		Headers     header_fields_;
		std::string message_body_;
	};
} // namespace cgi

#endif // RESPONSE_MESSAGE_HPP
