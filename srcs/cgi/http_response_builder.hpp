#ifndef HTTP_RESPONSE_BUILDER_HPP
#define HTTP_RESPONSE_BUILDER_HPP

#include "emptiable.hpp"
#include "header_value.hpp"
#include "response_message.hpp"
#include "result.hpp"

namespace cgi
{
	class HttpResponseBuilder
	{
	  private:
		enum ParseState {
			kParseHeader,
			kParseBody,
			kParseFinish,
		};

		typedef std::list<HeaderValue>              Values;
		typedef std::map<const std::string, Values> Headers;
		typedef std::list<FieldLine>                Lines;

		std::string header_str_;
		std::string message_body_;
		ParseState  state_;

	  public:
		HttpResponseBuilder();
		void                                     AddHeader(const std::string &header);
		void                                     AddBody(const std::string &str);
		Result<Emptiable<http::ResponseMessage>> GetHttpResponse(const std::string &line);

	  private:
		Result<http::ResponseMessage> Translate();
		Result<StatusLine>            TranslateStatusLine();
		Result<HeaderSection>         TranslateHeader();
	};

} // namespace cgi

#endif // HTTP_RESPONSE_BUILDER_HPP
