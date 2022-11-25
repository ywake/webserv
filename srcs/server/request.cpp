#include "request.hpp"

namespace server
{
	const http::RequestMessage Request::kEmptyMessage = http::RequestMessage();

	Request::Request(
		const http::RequestMessage *request_msg,
		const http::StatusCode     &error_code,
		ErrorType                   error_type
	)
		: request_msg_(request_msg), error_code_(error_code), error_type_(error_type)
	{}

	Request::Request(const Request &other)
		: request_msg_(other.request_msg_),
		  error_code_(other.error_code_),
		  error_type_(other.error_type_)
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
		if (request_msg_ == NULL) {
			return kEmptyMessage;
		}
		return *request_msg_;
	}

	const http::StatusCode &Request::GetErrStatusCode() const
	{
		return error_code_;
	}

	const Request::ErrorType &Request::GetErrorType() const
	{
		return error_type_;
	}

} // namespace server
