#include "request_line.hpp"

#include "absolute_form.hpp"
#include "asterisk_form.hpp"
#include "authority_form.hpp"
#include "error.hpp"
#include "origin_form.hpp"
#include "webserv_utils.hpp"

RequestLine::RequestLine() : method_(), request_target_(), http_version_() {}

/**
 * @brief
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

	std::vector<ThinString> array			= Split(request_line, " ");
	bool					is_invalid_size = array.size() != 3;
	if (is_invalid_size || !IsHttpVersion(array[2])) {
		throw Error("400");
	}
	method_			= array[0];
	request_target_ = TryConstructRequestTarget(array[1]);
	http_version_	= array[2];
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

bool RequestLine::IsHttpVersion(const ThinString &str)
{
	ThinString::ThinStrPair http_version_pair = str.DivideBy("/");
	ThinString				http			  = http_version_pair.first;
	ThinString				version			  = http_version_pair.second;
	if (http != "HTTP") {
		return false;
	}

	ThinString::ThinStrPair major_minor_pair = version.DivideBy(".", ThinString::kAlignRight);
	ThinString				major			 = major_minor_pair.first;
	ThinString				minor			 = major_minor_pair.second;
	return IsOneDigit(major) && IsOneDigit(minor);
}

bool RequestLine::operator==(const RequestLine &rhs) const
{
	return method_ == rhs.method_ && request_target_ == rhs.request_target_ &&
		   http_version_ == rhs.http_version_;
}
