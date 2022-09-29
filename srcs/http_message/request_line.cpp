#include "request_line.hpp"

#include "absolute_form.hpp"
#include "asterisk_form.hpp"
#include "authority_form.hpp"
#include "http_exceptions.hpp"
#include "origin_form.hpp"
#include "parse_abnf_core_rules.hpp"
#include "validate_request_line.hpp"
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
	bool       line_is_too_long   = request_line.len() > kMaxRequestLineLength;
	ThinString method_str         = request_line.substr(0, kMaxMethodLength + 1);
	bool       method_is_too_long = method_str.find(' ') > kMaxMethodLength;
	if (line_is_too_long || method_is_too_long) {
		throw http::BadRequestException();
	}

	enum {
		kMthodIdx = 0,
		kTargetIdx,
		kVersionIdx,
		kValidNumOfTokens
	};
	std::vector<ThinString> tokens          = Split(request_line, " ");
	bool                    is_invalid_size = tokens.size() != kValidNumOfTokens;
	if (is_invalid_size || !http_abnf::IsMethod(tokens[kMthodIdx]) ||
		!http_abnf::IsHttpVersion(tokens[kVersionIdx])) {
		throw http::BadRequestException();
	}
	method_         = tokens[kMthodIdx];
	request_target_ = TryConstructRequestTarget(tokens[kTargetIdx]);
	http_version_   = tokens[kVersionIdx];
}

// TODO methodを定数で定義したい
RequestTarget RequestLine::TryConstructRequestTarget(const ThinString &str)
{
	if (str.empty()) {
		throw http::BadRequestException();
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

bool RequestLine::operator==(const RequestLine &rhs) const
{
	return method_ == rhs.method_ && request_target_ == rhs.request_target_ &&
		   http_version_ == rhs.http_version_;
}
