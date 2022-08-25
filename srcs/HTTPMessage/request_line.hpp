#ifndef REQUEST_LINE_HPP
#define REQUEST_LINE_HPP
#include "request_target.hpp"
#include <string>

class RequestLine
{
  public:
	enum Methods { GET, POST, DELETE };
	Methods method_;
	RequestTarget request_target_;
	std::string http_version_;

	RequestLine();
	RequestLine(const std::string &request_line);
	RequestLine(Methods method, RequestTarget request_target, std::string http_version);

	bool operator==(const RequestLine &rhs) const;
};

#endif
