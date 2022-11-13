#ifndef HTTP_RESPONSE_BUILDER_HPP
#define HTTP_RESPONSE_BUILDER_HPP

#include "header_value.hpp"
#include "response_message.hpp"
#include "result.hpp"

namespace cgi
{
	class HttpResponseBuilder
	{
	  private:
		typedef std::list<HeaderValue>              Values;
		typedef std::map<const std::string, Values> Headers;

		std::string header_str_;
		std::string message_body_;

	  public:
		HttpResponseBuilder();
		void                          AddHeader(const std::string &header);
		void                          AddBody(const std::string &str);
		Result<http::ResponseMessage> Translate();

	  private:
		Result<void> TranslateHeader();
		Result<void> TranslateStatusLine();
		Result<void> TranslateBody();
	};

} // namespace cgi

#endif // HTTP_RESPONSE_BUILDER_HPP
