#ifndef REQUEST_LINE_HPP
#define REQUEST_LINE_HPP

#include <string>

#include "request_target.hpp"
#include "thin_string.hpp"

class RequestLine
{
  private:
	static const int kMaxRequestLineLength = 8192;
	static const int kMaxMethodLength      = 7;

  public:
	std::string   method_;
	RequestTarget request_target_;
	std::string   http_version_;

	RequestLine();
	RequestLine(const RequestLine &other);
	RequestLine(const ThinString &request_line);

	RequestLine(
		const std::string   &method,
		const RequestTarget &request_target,
		const std::string   &http_version
	);

	RequestLine &operator=(const RequestLine &rhs);
	bool         operator==(const RequestLine &rhs) const;

	void SetMethod(const std::string &method);
	void SetRequestTarget(const RequestTarget &request_target);
	void SetHttpVersion(const std::string &http_version);
	void SetHost(const std::string &host);
	void SetPort(const std::string &port);
	void SetPath(const std::string &path);
	void SetQuery(const std::string &query);

	const std::string   &GetMethod() const;
	const RequestTarget &GetRequestTarget() const;
	const std::string   &GetHttpVersion() const;

  private:
	RequestTarget TryConstructRequestTarget(const ThinString &str);
};

#endif
