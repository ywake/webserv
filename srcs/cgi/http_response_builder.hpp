#ifndef HTTP_RESPONSE_BUILDER_HPP
#define HTTP_RESPONSE_BUILDER_HPP

#include "emptiable.hpp"
#include "header_fields.hpp"
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

		enum ResponseType {
			kDocument,
			kLocalRedir,
			kClientRedir,
			kClientRedirWithDocument,
			kUndefined,
		};

		HeaderFields header_fields_;
		std::string  message_body_;
		ParseState   state_;

	  public:
		HttpResponseBuilder();
		Result<Emptiable<http::ResponseMessage>> GetHttpResponse(const std::string &line);

	  private:
		Result<http::ResponseMessage> Translate();
		Result<StatusLine>            TranslateStatusLine();
		Result<HeaderSection>         TranslateHeader();
		ResponseType                  IdentifyResponseType() const;
	};

} // namespace cgi

#endif // HTTP_RESPONSE_BUILDER_HPP
