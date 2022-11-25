#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "i_request.hpp"
#include "request_message.hpp"
#include "status_code.hpp"

namespace server
{
	class Request : public IRequest
	{
	  public:
		const http::RequestMessage *request_msg_;
		http::StatusCode            error_code_;
		ErrorType                   error_type_;

	  public:
		Request(
			const http::RequestMessage *request_msg = NULL,
			const http::StatusCode     &error_code  = http::StatusCode(),
			ErrorType                   error_type  = kNotError
		);
		Request(const Request &other);
		Request(const http::StatusCode &error_code, ErrorType error_type);
		void SetError(const http::StatusCode &error_code, ErrorType error_type);
		bool IsErr() const;
		bool IsFatal() const;

		const http::RequestMessage &GetMessage() const;
		const http::StatusCode     &GetErrStatusCode() const;
		const ErrorType            &GetErrorType() const;
	};
} // namespace server

#endif
