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

RequestLine::RequestLine(const RequestLine &other)
{
	*this = other;
}

// TODO refactor
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
	if (is_invalid_size || !http::abnf::IsMethod(tokens[kMthodIdx]) ||
		!http::abnf::IsHttpVersion(tokens[kVersionIdx])) {
		throw http::BadRequestException();
	}
	method_         = tokens[kMthodIdx].ToString();
	request_target_ = TryConstructRequestTarget(tokens[kTargetIdx]);
	http_version_   = tokens[kVersionIdx].ToString();
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
	const std::string &method, const RequestTarget &request_target, const std::string &http_version
)
	: method_(method), request_target_(request_target), http_version_(http_version)
{}

RequestLine &RequestLine::operator=(const RequestLine &rhs)
{
	if (this == &rhs) {
		return *this;
	}
	method_         = rhs.method_;
	request_target_ = rhs.request_target_;
	http_version_   = rhs.http_version_;
	return *this;
}

bool RequestLine::operator==(const RequestLine &rhs) const
{
	return method_ == rhs.method_ && request_target_ == rhs.request_target_ &&
		   http_version_ == rhs.http_version_;
}

void RequestLine::SetMethod(const std::string &method)
{
	method_ = method;
}

void RequestLine::SetRequestTarget(const RequestTarget &request_target)
{
	request_target_ = request_target;
}

void RequestLine::SetHttpVersion(const std::string &http_version)
{
	http_version_ = http_version;
}

const std::string &RequestLine::GetMethod() const
{
	return method_;
}

const RequestTarget &RequestLine::GetRequestTarget() const
{
	return request_target_;
}

const std::string &RequestLine::GetHttpVersion() const
{
	return http_version_;
}

void RequestLine::SetHost(const std::string &host)
{
	request_target_.SetHost(host);
}

void RequestLine::SetPort(const std::string &port)
{
	request_target_.SetPort(port);
}

void RequestLine::SetPath(const std::string &path)
{
	request_target_.SetPath(path);
}

void RequestLine::SetQuery(const std::string &query)
{
	request_target_.SetQuery(query);
}
