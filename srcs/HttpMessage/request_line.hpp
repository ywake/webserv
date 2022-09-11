#ifndef REQUEST_LINE_HPP
#define REQUEST_LINE_HPP
#include "request_target.hpp"
#include "thin_string.hpp"

class RequestLine
{
  private:
	static const int kMaxRequestLineLength = 8192;
	static const int kMaxMethodLength	   = 7;

  public:
	ThinString	  method_;
	RequestTarget request_target_;
	ThinString	  http_version_;

	RequestLine();
	RequestLine(const ThinString &request_line);

	RequestLine(
		const ThinString	 &method,
		const RequestTarget &request_target,
		const ThinString	 &http_version
	);

	bool operator==(const RequestLine &rhs) const;

	static bool IsHttpVersion(const ThinString &str);
	static bool IsMethod(const ThinString &str);

  private:
	RequestTarget TryConstructRequestTarget(const ThinString &str);
};

#endif
