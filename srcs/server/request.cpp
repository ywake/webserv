#include <cassert>

#include "request_parser.hpp"
namespace server
{
	RequestParser::Request::Request(
		const http::RequestMessage &request_msg,
		const http::StatusCode     &error_code,
		const ErrorType            &error_type
	)
		: request_msg_(request_msg), error_code_(error_code), error_type_(error_type)
	{}

	RequestParser::Request::Request(const Request &other)
		: request_msg_(other.request_msg_),
		  error_code_(other.error_code_),
		  error_type_(other.error_type_)
	{}

	RequestParser::Request::Request(const http::StatusCode &error_code, ErrorType error_type)
		: request_msg_(), error_code_(error_code), error_type_(error_type)
	{}

	RequestParser::Request::~Request() {}

	bool RequestParser::Request::IsErr() const
	{
		return error_type_ != kNotError;
	}

	bool RequestParser::Request::IsFatal() const
	{
		return error_type_ == kFatal;
	}

	bool RequestParser::Request::HasMessageBody() const
	{
		return request_msg_.HasMessageBody();
	}

	void RequestParser::Request::SetError(const http::StatusCode &error_code, ErrorType error_type)
	{
		error_code_ = error_code;
		error_type_ = error_type;
	}

	void RequestParser::Request::SetRequestLine(const RequestLine &request_line)
	{
		request_msg_.SetRequestLine(request_line);
	}

	void RequestParser::Request::SetHeaderSection(const HeaderSection &field_lines)
	{
		request_msg_.SetHeaderSection(field_lines);
	}

	void RequestParser::Request::SetBody(const std::string &body)
	{
		request_msg_.SetBody(body);
	}
	const std::string &RequestParser::Request::Method() const
	{
		return request_msg_.GetRequestLine().GetMethod();
	}

	const std::string &RequestParser::Request::Path() const
	{
		return request_msg_.GetRequestLine().GetRequestTarget().GetRequestFormData().path_;
	}

	const HeaderSection &RequestParser::Request::Headers() const
	{
		return request_msg_.GetHeaderSection();
	}

	const http::RequestMessage &RequestParser::Request::GetMessage() const
	{
		return request_msg_;
	}

	const http::StatusCode &RequestParser::Request::GetErrStatusCode() const
	{
		return error_code_;
	}

	const RequestParser::Request::ErrorType &RequestParser::Request::GetErrorType() const
	{
		return error_type_;
	}

	const std::vector<char> *RequestParser::Request::GetBody() const
	{
		return body;
	}

} // namespace server
