#include "http_exceptions.hpp"

namespace http
{
	HttpException::HttpException(StatusCode status_code)
		: std::exception(), status_code_(status_code)
	{}

	const StatusCode &HttpException::GetStatusCode() const
	{
		return status_code_;
	}

	const char *HttpException::what() const throw()
	{
		return status_code_.GetReasonPhrase().c_str();
	}

	BadRequestException::BadRequestException() : HttpException(StatusCode::kBadRequest) {}

	NotImplementedException::NotImplementedException() : HttpException(StatusCode::kNotImplemented)
	{}
} // namespace http
