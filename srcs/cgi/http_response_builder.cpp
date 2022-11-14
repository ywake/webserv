#include "http_response_builder.hpp"
#include "error.hpp"
#include "http_define.hpp"

using cgi::HttpResponseBuilder;

HttpResponseBuilder::HttpResponseBuilder() : message_body_() {}

Result<http::ResponseMessage> HttpResponseBuilder::Translate()
{
	Result<StatusLine>    res_status = TranslateStatusLine();
	Result<HeaderSection> res_header = TranslateHeader();

	if (res_status.IsErr()) {
		return Error(res_status.Err());
	}
	if (res_header.IsErr()) {
		return Error(res_header.Err());
	}
	return http::ResponseMessage(res_status.Val(), res_header.Val(), message_body_);
}

Result<StatusLine> HttpResponseBuilder::TranslateStatusLine()
{
	StatusLine               status;
	static const std::string http_version = "HTTP/1.1";

	//[TODO] エラーの時はbad requestを設定する必要がある
	status.status_code_ = "200";
	if (header_fields_.Contains("Status")) {
		HeaderFields::Values values = header_fields_.at("Status"); // atでlistを返す必要ある？
		status.status_code_ = values.front().GetValue();
	}
	status.http_version_ = http_version;
	return status;
}

Result<HeaderSection> HttpResponseBuilder::TranslateHeader()
{
	try {
		return HeaderSection(header_fields_.GetMap());
	} catch (const std::exception &e) {
		return Error("");
	}
}

// 可読性悪いので要修正
// これはサーバ側でやることかも
// | Response Type                          | Content-Type | local-Location | client-Location |
// | -------------------------------------- | ------------ | -------------- | --------------- |
// | Document Response                      | *            |                |                 |
// | Local Redirect Response                |              | *              |                 |
// | Client Redirect Response               |              |                | *               |
// | Client Redirect Response with Document | *            |                | *               |
HttpResponseBuilder::ResponseType HttpResponseBuilder::IdentifyResponseType() const
{
	bool has_content_type = header_fields_.Contains("Content-Type");
	bool has_location     = header_fields_.Contains("Location");

	if (!has_location) {
		return has_content_type ? kDocument : kUndefined;
	}
	HeaderFields::Values values = header_fields_.at("Location");
	if (values.empty()) {
		return kUndefined;
	}
	std::string value = values.front().GetValue();
	if (value.empty()) {
		return kUndefined;
	}
	bool is_local_location = value.at(0) == '/';
	if (is_local_location) {
		bool is_only_location = header_fields_.size() == 1;
		return is_only_location ? kLocalRedir : kUndefined;
	}
	bool has_status = header_fields_.Contains("Status");
	if (has_content_type && has_status) {
		return kClientRedirWithDocument;
	}
	return kUndefined;
}

Result<Emptiable<http::ResponseMessage>>
HttpResponseBuilder::GetHttpResponse(const std::string &line)
{
	bool is_blankline = line == http::kCrLf || line == http::kNl;

	switch (state_) {
	case kParseHeader:
		if (is_blankline) {
			state_ = kParseBody;
		} else {
			header_fields_.Add(line);
		}
		break;

	case kParseBody:
		if (is_blankline) {
			return Error("Invalid Cgi Response.");
		}
		if (line.empty()) {
			state_ = kParseFinish;
		}
		message_body_ += line;
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
