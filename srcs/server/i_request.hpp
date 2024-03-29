#ifndef I_REQUEST_HPP
#define I_REQUEST_HPP

#include <string>
#include <vector>

#include "field_section.hpp"
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

		virtual const std::string        &Method() const                     = 0;
		virtual const std::string        &Path() const                       = 0;
		virtual const std::string        &Query() const                      = 0;
		virtual const std::string        &Host() const                       = 0;
		virtual const std::string        &Port() const                       = 0;
		virtual std::string               Authority() const                  = 0;
		virtual const http::FieldSection &Headers() const                    = 0;
		virtual const RequestLine        &GetRequestLine() const             = 0;
		virtual const std::vector<char>  *GetBody() const                    = 0;
		virtual const http::StatusCode   &GetErrStatusCode() const           = 0;
		virtual const ErrorType          &GetErrorType() const               = 0;
		virtual bool                      NeedToClose() const                = 0;
		virtual bool                      HasMessageBody() const             = 0;
		virtual void                      SetPath(const std::string &path)   = 0;
		virtual void                      SetQuery(const std::string &query) = 0;
	};
} // namespace server

#endif
