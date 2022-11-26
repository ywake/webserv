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

	void RequestParser::Request::SetError(const http::StatusCode &error_code, ErrorType error_type)
	{
		error_code_ = error_code;
		error_type_ = error_type;
	}

	bool RequestParser::Request::IsErr() const
	{
		return error_type_ != kNotError;
	}

	bool RequestParser::Request::IsFatal() const
	{
		return error_type_ == kFatal;
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

} // namespace server
