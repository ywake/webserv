#include "http_response_builder.hpp"
#include "error.hpp"
#include "http_define.hpp"

using cgi::HttpResponseBuilder;

HttpResponseBuilder::HttpResponseBuilder() : header_str_(), message_body_() {}

void HttpResponseBuilder::AddHeader(const std::string &header)
{
	// std::size_t colon_pos = header.find(":");
	// if (colon_pos == ThinString::npos) {
	// 	throw http::BadRequestException();
	// }
	// ThinString name                 = header.substr(0, colon_pos);
	// ThinString value                = header.substr(colon_pos + 1);
	// header_fields_[name.ToString()] = value.ToString();
}

void HttpResponseBuilder::AddBody(const std::string &str)
{
	message_body_ += str;
}

Result<http::ResponseMessage> HttpResponseBuilder::Translate()
{
	Result<StatusLine>    res_status = TranslateStatusLine();
	Result<HeaderSection> res_header = TranslateHeader();

	if (res_status.IsErr() || res_header.IsErr()) {
		return Error();
	}
	return http::ResponseMessage(res_status.Val(), res_header.Val(), message_body_);
}

Result<StatusLine> HttpResponseBuilder::TranslateStatusLine()
{
	// StatusLine               status;
	// static const std::string http_version = "HTTP/1.1";
	//
	// response typeで分岐
	// header_fields_.find("Status");
	// status.http_version_ = http_version;
	// return status;
}

Result<HeaderSection> HttpResponseBuilder::TranslateHeader()
{
	try {
		HeaderSection headers(header_str_);
		return headers;
	} catch (const std::exception &e) {
		return Error("");
	}
}

Result<Emptiable<http::ResponseMessage>>
HttpResponseBuilder::GetHttpResponse(const std::string &line)
{
	bool is_blankline = line == http::kCrLf || line == http::kNl;

	switch (state_) {
	case kParseHeader:
		AddHeader(line);
		if (is_blankline) {
			state_ = kParseBody;
		}
		break;

	case kParseBody:
		if (is_blankline) {
			return Error("Invalid Cgi Response.");
		}
		if (line.empty()) {
			state_ = kParseFinish;
		}
		AddBody(line);
		break;

	case kParseFinish:
		Result<http::ResponseMessage> res = Translate();
		if (res.IsErr()) {
			return Error(res.Err());
		}
		state_ = kParseHeader;
		return Emptiable<http::ResponseMessage>(res.Val());
	}
	return Result<Emptiable<http::ResponseMessage>>();
}
