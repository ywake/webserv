#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "request_message.hpp"
#include "status_code.hpp"

namespace server
{
	class Request
	{
	  public:
		enum ErrorType {
			kNotError,
			kRecoverable,
			kFatal
		};

	  private:
		const http::RequestMessage *request_msg_;
		http::StatusCode            error_code_;
		ErrorType                   error_type_;

	  public:
		Request(const http::RequestMessage *request_msg);
		Request(const http::StatusCode &error_code, ErrorType error_type);
		void SetError(const http::StatusCode &error_code, ErrorType error_type);
		bool IsErr() const;
		bool IsFatal() const;

		const http::RequestMessage &GetMessage() const;
		const http::StatusCode     &GetErrStatusCode() const;
	};
} // namespace server

#endif
