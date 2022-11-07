#include "http_response_builder.hpp"
using cgi::HttpResponseBuilder;

HttpResponseBuilder::HttpResponseBuilder() : header_fields_(), message_body_() {}

void HttpResponseBuilder::AddHeader(const std::string &header)
{
	// keyとvalueに分解
}

void HttpResponseBuilder::AddBody(const std::string &str)
{
	message_body_ += str;
}

http::ResponseMessage HttpResponseBuilder::Translate() {}
