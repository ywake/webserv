#include "request_target.hpp"
#include <string>

RequestTarget::RequestTarget() : form_type_(UNDEFINED), uri_() {}

//[FIX]
//例外投げてるけど、Errorの有無を変数で持ってコンストラクタ呼び出し後にチェックの方が良い？
RequestTarget::RequestTarget(std::string uri)
{
	if (uri.empty()) {
		throw Error("400");
	}
	SpecifyForm(uri);
	switch (form_type_) {
	case ORIGIN:
		ParseOriginForm(uri);
		// uri_ = URI::ConstructFromOriginForm(uri);
		break;
	case ABSOLUTE:
		ParseAbsoluteForm(uri);
		break;
	case AUTHORITY:
		break;
	case ASTERISK:
		break;
	default:
		break;
	}
}

RequestTarget::RequestTarget(RequestForm form_type, URI request_target)
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

void RequestTarget::SpecifyForm(const std::string &uri)
{
	if (uri == "*") {
		form_type_ = ASTERISK;
	} else if (uri.at(0) == '/') {
		form_type_ = ORIGIN;
	} else if (uri.find("://") != std::string::npos) {
		form_type_ = ABSOLUTE;
	} else if (uri.find(":") != std::string::npos) {
		form_type_ = AUTHORITY;
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
