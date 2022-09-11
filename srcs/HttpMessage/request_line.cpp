#include "request_line.hpp"

#include "absolute_form.hpp"
#include "asterisk_form.hpp"
#include "authority_form.hpp"
#include "error.hpp"
#include "origin_form.hpp"
#include "parse_abnf_core_rules.hpp"
#include "webserv_utils.hpp"

RequestLine::RequestLine() : method_(), request_target_(), http_version_() {}

/**
 * @brief request-line = method SP request-target SP HTTP-version
 * @details メソッドの文字列長が長いときはパース時点でエラーにする
 * @details それ以外のメソッドのとき、実装しているメソッドかどうかはパースのあとで判定する
 * TODO: 文字列長の確認
 */
RequestLine::RequestLine(const ThinString &request_line)
	: method_(), request_target_(), http_version_()
{
	bool	   line_is_too_long	  = request_line.len() > kMaxRequestLineLength;
	ThinString method_str		  = request_line.substr(0, kMaxMethodLength + 1);
	bool	   method_is_too_long = method_str.find(' ') > kMaxMethodLength;
	if (line_is_too_long || method_is_too_long) {
		throw Error("400");
	}

	enum {
		kMthodIdx = 0,
		kTargetIdx,
		kVersionIdx,
		kValidNumOfTokens
	};
	std::vector<ThinString> tokens			= Split(request_line, " ");
	bool					is_invalid_size = tokens.size() != kValidNumOfTokens;
	if (is_invalid_size || !IsMethod(tokens[kMthodIdx]) || !IsHttpVersion(tokens[kVersionIdx])) {
		throw Error("400");
	}
	method_			= tokens[kMthodIdx];
	request_target_ = TryConstructRequestTarget(tokens[kTargetIdx]);
	http_version_	= tokens[kVersionIdx];
}

RequestTarget RequestLine::TryConstructRequestTarget(const ThinString &str)
{
	if (str.empty()) {
		throw Error("400");
	} else if (method_ == "CONNECT") {
		return RequestTarget(AuthorityForm(str));
	} else if (method_ == "OPTIONS" && str == "*") {
		return RequestTarget(AsteriskForm(str));
	} else if (str.at(0) == '/') {
		return RequestTarget(OriginForm(str));
	} else {
		return RequestTarget(AbsoluteForm(str));
	}
}

RequestLine::RequestLine(
	const ThinString &method, const RequestTarget &request_target, const ThinString &http_version
)
	: method_(method), request_target_(request_target), http_version_(http_version)
{}

static bool IsOneDigit(const ThinString &str)
{
	return str.len() == 1 && std::isdigit(str.at(0));
}

bool RequestLine::IsMethod(const ThinString &str)
{
	return ABNF::IsTcharOnly(str);
}

// HTTP-version = HTTP-name "/" DIGIT "." DIGIT
bool RequestLine::IsHttpVersion(const ThinString &str)
{
	ThinString::ThinStrPair http_version_pair = str.DivideBy("/", ThinString::kKeepDelimLeft);
	ThinString				version			  = http_version_pair.second;
	ThinString::ThinStrPair major_minor_pair  = version.DivideBy(".", ThinString::kAlignRight);
	ThinString				http			  = http_version_pair.first;
	ThinString				major			  = major_minor_pair.first;
	ThinString				minor			  = major_minor_pair.second;
	return http == "HTTP/" && IsOneDigit(major) && IsOneDigit(minor);
}

bool RequestLine::operator==(const RequestLine &rhs) const
{
	return method_ == rhs.method_ && request_target_ == rhs.request_target_ &&
		   http_version_ == rhs.http_version_;
}
