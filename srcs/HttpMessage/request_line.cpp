#include "request_line.hpp"

#include "absolute_form.hpp"
#include "asterisk_form.hpp"
#include "authority_form.hpp"
#include "error.hpp"
#include "origin_form.hpp"

RequestLine::RequestLine() : method_(), request_target_(), http_version_() {}

RequestLine::RequestLine(const std::string &request_line)
	: method_(), request_target_(), http_version_()
{
	(void)request_line;
}

void RequestLine::ParseRequestTarget(const ThinString &str)
{
	switch (method_) {
	case OPTIONS:
		request_target_ = RequestTarget(AsteriskForm(str));
		break;
	case CONNECT:
		request_target_ = RequestTarget(AuthorityForm(str));
		break;
	default:
		ParseRequestTargetForRegularMeshods(str);
		break;
	}
}

void RequestLine::ParseRequestTargetForRegularMeshods(const ThinString &str)
{
	// TODO 空文字列のハンドリングをする責任が誰にあるのか微妙
	// そもそもここに空文字列が来ることはないかもだけど。
	// elseでabsoluteFormに入れば自動的に弾かれるけど、そもそも空文字列でabsoluteformに入るべきなのかが分からない
	if (str.empty()) {
		throw Error("400");
	} else if (str.at(0) == '/') {
		request_target_ = RequestTarget(OriginForm(str));
	} else {
		request_target_ = RequestTarget(AbsoluteForm(str));
	}
}

RequestLine::RequestLine(Methods method, RequestTarget request_target, std::string http_version)
	: method_(method), request_target_(request_target), http_version_(http_version)
{}

bool RequestLine::operator==(const RequestLine &rhs) const
{
	return method_ == rhs.method_ && request_target_ == rhs.request_target_ &&
		   http_version_ == rhs.http_version_;
}
