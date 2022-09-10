#include "request_line.hpp"

#include "absolute_form.hpp"
#include "asterisk_form.hpp"
#include "authority_form.hpp"
#include "origin_form.hpp"

RequestLine::RequestLine() : method_(), request_target_(), http_version_() {}

RequestLine::RequestLine(const std::string &request_line)
	: method_(), request_target_(), http_version_()
{
	(void)request_line;
}

void RequestLine::ParseRequestTarget(const ThinString &str)
{
	switch (method_) {
	case OPTIONS:
		request_target_ = RequestTarget(AuthorityForm(str));
		break;
	case CONNECT:
		request_target_ = RequestTarget(AsteriskForm(str));
		break;
	default:
		break;
	}
}

RequestLine::RequestLine(Methods method, RequestTarget request_target, std::string http_version)
	: method_(method), request_target_(request_target), http_version_(http_version)
{}

bool RequestLine::operator==(const RequestLine &rhs) const
{
	return method_ == rhs.method_ && request_target_ == rhs.request_target_ &&
		   http_version_ == rhs.http_version_;
}
