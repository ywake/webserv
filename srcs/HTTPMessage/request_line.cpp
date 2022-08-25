#include "request_line.hpp"

RequestLine::RequestLine() : method_(), request_target_(), http_version_() {}

RequestLine::RequestLine(const std::string &request_line)
	: method_(), request_target_(), http_version_()
{
	(void)request_line;
}

RequestLine::RequestLine(Methods method, RequestTarget request_target, std::string http_version)
	: method_(method), request_target_(request_target), http_version_(http_version)
{
}

bool RequestLine::operator==(const RequestLine &rhs) const
{
	return method_ == rhs.method_ && request_target_ == rhs.request_target_ &&
		   http_version_ == rhs.http_version_;
}
