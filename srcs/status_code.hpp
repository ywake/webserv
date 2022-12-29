#ifndef STATUS_CODE_HPP
#define STATUS_CODE_HPP

#include <map>
#include <string>

#include "emptiable.hpp"

namespace http
{
	class StatusCode
	{
	  public:
		typedef unsigned int Code;
		enum DefinedCode {
			kContinue                    = 100,
			kSwitchingProtocols          = 101,
			kOK                          = 200,
			kCreated                     = 201,
			kAccepted                    = 202,
			kNonAuthoritativeInformation = 203,
			kNoContent                   = 204,
			kResetContent                = 205,
			kPartialContent              = 206,
			kMultipleChoices             = 300,
			kMovedPermanently            = 301,
			kFound                       = 302,
			kSeeOther                    = 303,
			kNotModified                 = 304,
			kUseProxy                    = 305,
			kTemporaryRedirect           = 307,
			kPermanentRedirect           = 308,
			kBadRequest                  = 400,
			kUnauthorized                = 401,
			kPaymentRequired             = 402,
			kForbidden                   = 403,
			kNotFound                    = 404,
			kMethodNotAllowed            = 405,
			kNotAcceptable               = 406,
			kProxyAuthenticationRequired = 407,
			kRequestTimeout              = 408,
			kConflict                    = 409,
			kGone                        = 410,
			kLengthRequired              = 411,
			kPreconditionFailed          = 412,
			kContentTooLarge             = 413,
			kURITooLong                  = 414,
			kUnsupportedMediaType        = 415,
			kRangeNotSatisfiable         = 416,
			kExpectationFailed           = 417,
			kMisdirectedRequest          = 421,
			kUnprocessableContent        = 422,
			kUpgradeRequired             = 426,
			kInternalServerError         = 500,
			kNotImplemented              = 501,
			kBadGateway                  = 502,
			kServiceUnavailable          = 503,
			kGatewayTimeout              = 504,
			kHTTPVersionNotSupported     = 505
		};

	  private:
		typedef std::map<const Code, std::string> ReasonPhrase;

	  private:
		static const ReasonPhrase reason_phrase_;
		static const Code         kUndefinedCode;
		Code                      code_;
		Emptiable<std::string>    phrase_;

	  public:
		StatusCode(Code code = kUndefinedCode);
		StatusCode(Code code, const std::string &phrase);
		~StatusCode() throw();

		Code               GetCode() const;
		std::string        GetCodeStr() const;
		const std::string &GetReasonPhrase() const;

	  private:
		static ReasonPhrase InitReasonPhrase();
	};
} // namespace http

#endif
