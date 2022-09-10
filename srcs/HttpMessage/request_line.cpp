#include "request_line.hpp"

#include "absolute_form.hpp"
#include "asterisk_form.hpp"
#include "authority_form.hpp"
#include "error.hpp"
#include "origin_form.hpp"

RequestLine::RequestLine() : method_(), request_target_(), http_version_() {}

RequestLine::RequestLine(const std::string &request_line)
	: method_(), request_target_(), http_version_()
{
	(void)request_line;
}

RequestTarget RequestLine::ParseRequestTarget(const ThinString &str)
{
	if (str.empty()) {
		throw Error("400");
	} else if (method_ == OPTIONS) {
		return RequestTarget(AsteriskForm(str));
	} else if (method_ == CONNECT)
		return RequestTarget(AuthorityForm(str));
	else if (str.at(0) == '/') {
		return RequestTarget(OriginForm(str));
	} else {
		return RequestTarget(AbsoluteForm(str));
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
