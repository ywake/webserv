#include "request_target.hpp"
#include "absolute_form.hpp"
#include "asterisk_form.hpp"
#include "authority_form.hpp"
#include "error.hpp"
#include "http_exceptions.hpp"
#include "origin_form.hpp"
#include "parse_http_utils.hpp"
#include "parse_path.hpp"
#include "parse_uri.hpp"
#include "percent_encode.hpp"
#include "thin_string.hpp"
#include "webserv_utils.hpp"

#include <string>
#include <vector>

RequestTarget::RequestTarget() : form_type_(), form_data_() {}

RequestTarget::RequestTarget(const RequestTarget &other) : form_type_(), form_data_()
{
	*this = other;
}

RequestTarget::RequestTarget(const OriginForm &form) : form_data_()
{
	Result<std::string> path = http::NormalizePath(TryPercentDecode(form.GetPath()));
	if (path.IsErr()) {
		throw http::BadRequestException();
	}
	form_type_        = kOriginForm;
	form_data_.path_  = path.Val();
	form_data_.query_ = form.GetQuery().ToString();
}

RequestTarget::RequestTarget(const AbsoluteForm &form) : form_data_()
{
	Result<std::string> path = http::NormalizePath(TryPercentDecode(form.GetPath()));
	if (path.IsErr()) {
		throw http::BadRequestException();
	}
	form_type_           = kAbsoluteForm;
	form_data_.scheme_   = utils::ToLowerString(form.GetScheme().ToString());
	form_data_.userinfo_ = TryPercentDecode(form.GetUserinfo());
	form_data_.host_     = utils::ToLowerString(TryPercentDecode(form.GetHost()));
	form_data_.port_     = form.GetPort().ToString();
	form_data_.path_     = path.Val();
	form_data_.query_    = form.GetQuery().ToString();
}

RequestTarget::RequestTarget(const AuthorityForm &form) : form_data_()
{
	form_type_       = kAuthorityForm;
	form_data_.host_ = utils::ToLowerString(TryPercentDecode(form.GetHost()));
	form_data_.port_ = form.GetPort().ToString();
}

RequestTarget::RequestTarget(const AsteriskForm &form) : form_data_()
{
	(void)form;
	form_type_ = kAsteriskForm;
}

RequestTarget::RequestTarget(const RequestFormData &request_target, FormType form_type)
	: form_type_(form_type), form_data_(request_target)
{}

std::string RequestTarget::TryPercentDecode(const ThinString &s)
{
	Result<std::string> decoded = uri::PercentDecode(s);
	if (decoded.IsErr()) {
		throw http::BadRequestException();
	}
	return decoded.Val();
}

RequestTarget &RequestTarget::operator=(const RequestTarget &other)
{
	if (this == &other) {
		return *this;
	}
	form_type_ = other.form_type_;
	form_data_ = other.form_data_;
	return *this;
}

bool RequestTarget::operator==(const RequestTarget &rhs) const
{
	if (this == &rhs) {
		return true;
	}
	return form_type_ == rhs.form_type_ && form_data_ == rhs.form_data_;
}

bool RequestTarget::operator!=(const RequestTarget &rhs) const
{
	return !(*this == rhs);
}

const RequestFormData &RequestTarget::GetRequestFormData() const
{
	return form_data_;
}

RequestTarget::FormType RequestTarget::GetFormType() const
{
	return form_type_;
}

void RequestTarget::SetHost(const std::string &host)
{
	form_data_.host_ = host;
}

void RequestTarget::SetPort(const std::string &port)
{
	form_data_.port_ = port;
}

void RequestTarget::SetPath(const std::string &path)
{
	form_data_.path_ = path;
}

void RequestTarget::SetQuery(const std::string &query)
{
	form_data_.query_ = query;
}

std::ostream &operator<<(std::ostream &os, const RequestTarget &request_target)
{
	os << "\n" << request_target.GetRequestFormData();
	std::string form_type;
	switch (request_target.GetFormType()) {
	case RequestTarget::kOriginForm:
		form_type = "OriginForm";
		break;
	case RequestTarget::kAbsoluteForm:
		form_type = "AbsoluteForm";
		break;
	case RequestTarget::kAuthorityForm:
		form_type = "AuthorityForm";
		break;
	case RequestTarget::kAsteriskForm:
		form_type = "AsteriskForm";
		break;
	}
	os << "\n" << form_type;
	return os;
}
