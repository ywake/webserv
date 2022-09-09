#include "request_target.hpp"

#include <string>

#include "absolute_uri.hpp"
#include "i_target_form.hpp"

RequestTarget::RequestTarget() : form_type_(UNDEFINED), uri_() {}

//[FIX]
//例外投げてるけど、Errorの有無を変数で持ってコンストラクタ呼び出し後にチェックの方が良い？
RequestTarget::RequestTarget(std::string uri) : uri_()
{
	if (uri.empty()) {
		throw Error("400");
	}
	form_type_ = SpecifyForm(uri);
	switch (form_type_) {
	case ORIGIN: {
		// OriginForm origin(uri);
		// SetUri(&origin);
		break;
	}
	case ABSOLUTE: { // variables cannot be declared in a switch statement, so enclosed in a block
		typedef AbsoluteUri
			AbsoluteForm; // TODO: HTTPエラーを処理するためにAbsoluteFormクラスを作る
		AbsoluteForm absolute(uri);
		SetUri(&absolute);
		break;
	}
	case AUTHORITY:
		break;
	case ASTERISK:
		break;
	case UNDEFINED:
		break;
	default:
		break;
	}
}

RequestTarget::RequestTarget(RequestForm form_type, Uri request_target)
	: form_type_(form_type), uri_(request_target)
{
}

bool RequestTarget::operator==(const RequestTarget &rhs) const
{
	return form_type_ == rhs.form_type_ && uri_ == rhs.uri_;
}

bool RequestTarget::operator!=(const RequestTarget &rhs) const
{
	return !(*this == rhs);
}

RequestTarget::RequestForm RequestTarget::SpecifyForm(const std::string &uri)
{
	if (uri == "*") {
		return ASTERISK;
	} else if (uri.at(0) == '/') {
		return ORIGIN;
	} else if (uri.find("://") != std::string::npos) {
		return ABSOLUTE;
	} else if (uri.find(":") != std::string::npos) {
		return AUTHORITY;
	} else {
		throw Error("400");
	}
}

// origin-form
// = absolute-path [ "?" query ]
void RequestTarget::ParseOriginForm(std::string uri)
{
	ThinString thin(uri);
	ThinString::ThinStrPair path_query = thin.DivideBy("?");

	if (!ABNF::IsPathAbsolute(path_query.first) || !ABNF::IsQuery(path_query.second)) {
		throw Error("400");
	}
	uri_.path_ = path_query.first.ToString();
	uri_.query_ = path_query.second.ToString();
}

// absolute-form = absolute-URI
void RequestTarget::ParseAbsoluteForm(ThinString uri)
{
	ThinString::ThinStrPair scheme_heir = uri.DivideBy(":");
	ThinString::ThinStrPair heir_query = scheme_heir.second.DivideBy("?");
	if (!ABNF::IsScheme(scheme_heir.first) || !ABNF::IsHierPart(heir_query.first) ||
		!ABNF::IsQuery(heir_query.second)) {
		throw Error("400");
	}
	uri_.scheme_ = scheme_heir.first.ToString();
}

void RequestTarget::SetUri(ITargetForm *form)
{
	uri_.scheme_ = form->GetScheme();
	uri_.userinfo_ = form->GetUserinfo();
	uri_.host_ = form->GetHost();
	uri_.port_ = form->GetPort();
	uri_.path_ = form->GetPath();
	uri_.query_ = form->GetQuery();
}

std::ostream &operator<<(std::ostream &os, const RequestTarget &request_target)
{
	switch (request_target.form_type_) {
	case RequestTarget::UNDEFINED:
		os << "UNDEFINED";
		break;
	case RequestTarget::ORIGIN:
		os << "ORIGIN";
		break;
	case RequestTarget::ABSOLUTE:
		os << "ABSOLUTE";
		break;
	case RequestTarget::AUTHORITY:
		os << "AUTHORITY";
		break;
	case RequestTarget::ASTERISK:
		os << "ASTERISK";
		break;
	}
	os << "\n" << request_target.uri_;
	return os;
}
