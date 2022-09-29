#include "status_code.hpp"

namespace http
{
	StatusCode::StatusCode(Code code) : code_(code) {}

	StatusCode::StatusCode(const EmptyStatusCode &code) : code_(kUndefinedCode)
	{
		(void)code;
	}

	bool StatusCode::empty()
	{
		return code_ == kUndefinedCode;
	}

	const std::map<const StatusCode::Code, const std::string> StatusCode::reason_phrase_(
		{{kUndefinedCode, ""},
		 {kContinue, "Continue"},
		 {kSwitchingProtocols, "Switching Protocols"},
		 {kOK, "OK"},
		 {kCreated, "Created"},
		 {kAccepted, "Accepted"},
		 {kNonAuthoritativeInformation, "Non-Authoritative Information"},
		 {kNoContent, "No Content"},
		 {kResetContent, "Reset Content"},
		 {kPartialContent, "Partial Content"},
		 {kMultipleChoices, "Multiple Choices"},
		 {kMovedPermanently, "Moved Permanently"},
		 {kFound, "Found"},
		 {kSeeOther, "See Other"},
		 {kNotModified, "Not Modified"},
		 {kUseProxy, "Use Proxy"},
		 {kTemporaryRedirect, "Temporary Redirect"},
		 {kPermanentRedirect, "Permanent Redirect"},
		 {kBadRequest, "Bad Request"},
		 {kUnauthorized, "Unauthorized"},
		 {kPaymentRequired, "Payment Required"},
		 {kForbidden, "Forbidden"},
		 {kNotFound, "Not Found"},
		 {kMethodNotAllowed, "Method Not Allowed"},
		 {kNotAcceptable, "Not Acceptable"},
		 {kProxyAuthenticationRequired, "Proxy Authentication Required"},
		 {kRequestTimeout, "Request Timeout"},
		 {kConflict, "Conflict"},
		 {kGone, "Gone"},
		 {kLengthRequired, "Length Required"},
		 {kPreconditionFailed, "Precondition Failed"},
		 {kContentTooLarge, "Content Too Large"},
		 {kURITooLong, "URI Too Long"},
		 {kUnsupportedMediaType, "Unsupported Media Type"},
		 {kRangeNotSatisfiable, "Range Not Satisfiable"},
		 {kExpectationFailed, "Expectation Failed"},
		 {kMisdirectedRequest, "Misdirected Request"},
		 {kUnprocessableContent, "Unprocessable Content"},
		 {kUpgradeRequired, "Upgrade Required"},
		 {kInternalServerError, "Internal Server Error"},
		 {kNotImplemented, "Not Implemented"},
		 {kBadGateway, "Bad Gateway"},
		 {kServiceUnavailable, "Service Unavailable"},
		 {kGatewayTimeout, "Gateway Timeout"},
		 {kHTTPVersionNotSupported, "HTTP Version Not Supported"}}
	);

	const std::string &StatusCode::GetReasonPhrase() const
	{
		try {
			return reason_phrase_.at(code_);
		} catch (const std::exception &e) {
			return reason_phrase_.at(kUndefinedCode);
		}
	}

} // namespace http
