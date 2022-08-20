#ifndef REQUEST_LINE_HPP
#define REQUEST_LINE_HPP
#include "request_target.hpp"
#include <string>

class RequestLine
{
  public:
	enum Methods {
		GET,
		POST,
		DELETE,
	};
	Methods method_;
	RequestTarget request_target_;
	std::string http_version_;

	RequestLine() : method_(), request_target_(), http_version_() {}
	RequestLine(const std::string &request_line) : method_(), request_target_(), http_version_() {}

	RequestLine(Methods method, RequestTarget request_target, std::string http_version)
		: method_(method), request_target_(request_target), http_version_(http_version)
	{
	}

	bool operator==(const RequestLine &rhs) const
	{
		return method_ == rhs.method_ && request_target_ == rhs.request_target_ &&
			   http_version_ == rhs.http_version_;
	}
};

#endif
