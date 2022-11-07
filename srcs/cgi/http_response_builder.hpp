#ifndef HTTP_RESPONSE_BUILDER_HPP
#define HTTP_RESPONSE_BUILDER_HPP

#include "header_value.hpp"
#include "response_message.hpp"

namespace cgi
{
	class HttpResponseBuilder
	{
	  private:
		typedef std::list<HeaderValue>              Values;
		typedef std::map<const std::string, Values> Headers;

		Headers     header_fields_;
		std::string message_body_;

	  public:
		HttpResponseBuilder();
		void                  AddHeader(const std::string &header);
		void                  AddBody(const std::string &str);
		http::ResponseMessage Translate();
	};

} // namespace cgi

#endif // HTTP_RESPONSE_BUILDER_HPP
