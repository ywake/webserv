#include "status_code.hpp"
#include <stdexcept>

#include "webserv_utils.hpp"

namespace http
{
	const StatusCode::ReasonPhrase StatusCode::reason_phrase_ = StatusCode::InitReasonPhrase();
	const StatusCode::Code         StatusCode::kMax           = 599;
	const StatusCode::Code         StatusCode::kMin           = 100;
	const std::size_t              StatusCode::kCodeDigits    = 3;
	const StatusCode::Code         StatusCode::kUndefinedCode = 0;

	StatusCode::StatusCode(Code code) : code_(code), phrase_() {}

	StatusCode::StatusCode(Code code, const std::string &phrase) : code_(code), phrase_(phrase) {}

	StatusCode::~StatusCode() throw() {}

	StatusCode::Code StatusCode::GetCode() const
	{
		return code_;
	}

	const std::string &StatusCode::GetReasonPhrase() const
	{
		if (!phrase_.empty()) {
			return phrase_.Value();
		}
		try {
			return reason_phrase_.at(code_);
		} catch (const std::out_of_range &e) {
			return reason_phrase_.at(kUndefinedCode);
		}
	}

	std::string StatusCode::GetCodeStr() const
	{
		return utils::ToString(code_);
	}

	StatusCode::ReasonPhrase StatusCode::InitReasonPhrase()
	{
		ReasonPhrase reason_phrase;

		reason_phrase[kUndefinedCode]               = "";
		reason_phrase[kContinue]                    = "Continue";
		reason_phrase[kSwitchingProtocols]          = "Switching Protocols";
		reason_phrase[kOK]                          = "OK";
		reason_phrase[kCreated]                     = "Created";
		reason_phrase[kAccepted]                    = "Accepted";
		reason_phrase[kNonAuthoritativeInformation] = "Non-Authoritative Information";
		reason_phrase[kNoContent]                   = "No Content";
		reason_phrase[kResetContent]                = "Reset Content";
		reason_phrase[kPartialContent]              = "Partial Content";
		reason_phrase[kMultipleChoices]             = "Multiple Choices";
		reason_phrase[kMovedPermanently]            = "Moved Permanently";
		reason_phrase[kFound]                       = "Found";
		reason_phrase[kSeeOther]                    = "See Other";
		reason_phrase[kNotModified]                 = "Not Modified";
		reason_phrase[kUseProxy]                    = "Use Proxy";
		reason_phrase[kTemporaryRedirect]           = "Temporary Redirect";
		reason_phrase[kPermanentRedirect]           = "Permanent Redirect";
		reason_phrase[kBadRequest]                  = "Bad Request";
		reason_phrase[kUnauthorized]                = "Unauthorized";
		reason_phrase[kPaymentRequired]             = "Payment Required";
		reason_phrase[kForbidden]                   = "Forbidden";
		reason_phrase[kNotFound]                    = "Not Found";
		reason_phrase[kMethodNotAllowed]            = "Method Not Allowed";
		reason_phrase[kNotAcceptable]               = "Not Acceptable";
		reason_phrase[kProxyAuthenticationRequired] = "Proxy Authentication Required";
		reason_phrase[kRequestTimeout]              = "Request Timeout";
		reason_phrase[kConflict]                    = "Conflict";
		reason_phrase[kGone]                        = "Gone";
		reason_phrase[kLengthRequired]              = "Length Required";
		reason_phrase[kPreconditionFailed]          = "Precondition Failed";
		reason_phrase[kContentTooLarge]             = "Content Too Large";
		reason_phrase[kURITooLong]                  = "URI Too Long";
		reason_phrase[kUnsupportedMediaType]        = "Unsupported Media Type";
		reason_phrase[kRangeNotSatisfiable]         = "Range Not Satisfiable";
		reason_phrase[kExpectationFailed]           = "Expectation Failed";
		reason_phrase[kMisdirectedRequest]          = "Misdirected Request";
		reason_phrase[kUnprocessableContent]        = "Unprocessable Content";
		reason_phrase[kUpgradeRequired]             = "Upgrade Required";
		reason_phrase[kInternalServerError]         = "Internal Server Error";
		reason_phrase[kNotImplemented]              = "Not Implemented";
		reason_phrase[kBadGateway]                  = "Bad Gateway";
		reason_phrase[kServiceUnavailable]          = "Service Unavailable";
		reason_phrase[kGatewayTimeout]              = "Gateway Timeout";
		reason_phrase[kHTTPVersionNotSupported]     = "HTTP Version Not Supported";

		return reason_phrase;
	}

	bool StatusCode::IsValidCode(Code code)
	{
		return kMin <= code && code <= kMax;
	}

	bool StatusCode::IsValidCode(const std::string &code)
	{
		if (code.size() != kCodeDigits) {
			return false;
		}
		Result<unsigned long> num = utils::StrToUnsignedLong(code);
		return num.IsOk() && kMin <= num.Val() && num.Val() <= kMax;
	}

	bool StatusCode::operator==(const Code &code) const
	{
		return code_ == code;
	}

} // namespace http
