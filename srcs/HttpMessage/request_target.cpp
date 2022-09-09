#include "request_target.hpp"
#include "absolute_form.hpp"
#include "authority_form.hpp"
#include "error.hpp"
#include "origin_form.hpp"
#include "parse_path.hpp"
#include "parse_uri.hpp"
#include "thin_string.hpp"

#include <string>
#include <vector>

RequestTarget::RequestTarget() : form_type_(UNDEFINED), form_data_() {}

//[Q]
//例外投げてるけど、Errorの有無を変数で持ってコンストラクタ呼び出し後にチェックの方が良い？
RequestTarget::RequestTarget(std::string uri) : form_data_()
{
	if (uri.empty()) {
		throw Error("400");
	}
	form_type_ = SpecifyForm(uri);
	switch (form_type_) {
	case ORIGIN: {
		OriginForm origin(uri);
		SetFormData(&origin);
		break;
	}
	case ABSOLUTE: { // variables cannot be declared in a switch statement, so enclosed in a block
		AbsoluteForm absolute(uri);
		SetFormData(&absolute);
		break;
	}
	case AUTHORITY: {
		AuthorityForm authority(uri);
		SetFormData(&authority);
		break;
	}
	case ASTERISK:
		break;
	case UNDEFINED:
		break;
	default:
		break;
	}
}

RequestTarget::RequestTarget(RequestForm form_type, RequestFormData request_target)
	: form_type_(form_type), form_data_(request_target)
{
}

bool RequestTarget::operator==(const RequestTarget &rhs) const
{
	return form_type_ == rhs.form_type_ && form_data_ == rhs.form_data_;
}

bool RequestTarget::operator!=(const RequestTarget &rhs) const
{
	return !(*this == rhs);
}

//[TODO] メソッドの種類でフォームを判別する
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

void RequestTarget::SetFormData(ITargetForm *form)
{
	form_data_.scheme_ = form->GetScheme();
	form_data_.userinfo_ = form->GetUserinfo();
	form_data_.host_ = form->GetHost();
	form_data_.port_ = form->GetPort();
	form_data_.path_ = form->GetPath();
	form_data_.query_ = form->GetQuery();
}

RequestTarget::RequestForm RequestTarget::GetRequestForm() const
{
	return form_type_;
}

std::ostream &operator<<(std::ostream &os, const RequestTarget &request_target)
{
	switch (request_target.GetRequestForm()) {
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
	os << "\n" << request_target.GetRequestForm();
	return os;
}
