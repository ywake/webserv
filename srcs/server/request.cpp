#include "request.hpp"

namespace sever
{
	Request::Request(const http::RequestMessage *request_msg)
		: request_msg_(request_msg), error_code_(), error_type_(kNotError)
	{}

	Request::Request(const http::StatusCode &error_code, ErrorType error_type)
		: request_msg_(NULL), error_code_(error_code), error_type_(error_type)
	{}

	void Request::SetError(const http::StatusCode &error_code, ErrorType error_type)
	{
		error_code_ = error_code;
		error_type_ = error_type;
	}

	bool Request::IsErr() const
	{
		return error_type_ != kNotError;
	}

	bool Request::IsFatal() const
	{
		return error_type_ == kFatal;
	}

	const http::RequestMessage &Request::GetMessage() const
	{
		return *request_msg_;
	}

	const http::StatusCode &Request::GetErrStatusCode() const
	{
		return error_code_;
	}
} // namespace sever
