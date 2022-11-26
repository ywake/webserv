#ifndef I_REQUEST_HPP
#define I_REQUEST_HPP

#include "request_message.hpp"
#include "status_code.hpp"

namespace server
{
	class IRequest
	{
	  public:
		enum ErrorType {
			kNotError,
			kRecoverable,
			kFatal
		};

	  public:
		virtual ~IRequest(){};

		virtual void SetError(const http::StatusCode &error_code, ErrorType error_type) = 0;
		virtual bool IsErr() const                                                      = 0;
		virtual bool IsFatal() const                                                    = 0;

		virtual const http::RequestMessage &GetMessage() const       = 0;
		virtual const http::StatusCode     &GetErrStatusCode() const = 0;
		virtual const ErrorType            &GetErrorType() const     = 0;
	};
} // namespace server

#endif
