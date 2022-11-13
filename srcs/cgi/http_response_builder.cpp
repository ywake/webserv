#include "http_response_builder.hpp"
#include "error.hpp"

using cgi::HttpResponseBuilder;

HttpResponseBuilder::HttpResponseBuilder() : header_str_(), message_body_() {}

void HttpResponseBuilder::AddHeader(const std::string &header)
{
	header_str_ += header;
}

void HttpResponseBuilder::AddBody(const std::string &str)
{
	message_body_ += str;
}

Result<http::ResponseMessage> HttpResponseBuilder::Translate()
{
	Result<void> res = TranslateStatusLine();
	if (res.IsErr()) {
		return Error(res.Err());
	}
}

Result<void> HttpResponseBuilder::TranslateHeader()
{
	return;
}

Result<void> HttpResponseBuilder::TranslateStatusLine()
{
	return;
}

Result<void> HttpResponseBuilder::TranslateBody()
{
	return;
}
