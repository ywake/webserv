#include "request_target.hpp"
#include "absolute_form.hpp"
#include "asterisk_form.hpp"
#include "authority_form.hpp"
#include "error.hpp"
#include "http_exceptions.hpp"
#include "origin_form.hpp"
#include "parse_path.hpp"
#include "parse_uri.hpp"
#include "thin_string.hpp"
#include "webserv_utils.hpp"

#include <string>
#include <vector>

RequestTarget::RequestTarget() : form_data_() {}

RequestTarget::RequestTarget(const OriginForm &form) : form_data_()
{
	form_type_        = kOriginForm;
	form_data_.path_  = form.GetPath().ToString();
	form_data_.query_ = form.GetQuery().ToString();
}

RequestTarget::RequestTarget(const AbsoluteForm &form) : form_data_()
{
	form_type_           = kAbsoluteForm;
	form_data_.scheme_   = utils::ToLowerString(form.GetScheme().ToString());
	form_data_.userinfo_ = form.GetUserinfo().ToString();
	form_data_.host_     = utils::ToLowerString(form.GetHost().ToString());
	form_data_.port_     = form.GetPort().ToString();
	form_data_.path_     = form.GetPath().ToString();
	form_data_.query_    = form.GetQuery().ToString();
}

RequestTarget::RequestTarget(const AuthorityForm &form) : form_data_()
{
	form_type_       = kAuthorityForm;
	form_data_.host_ = utils::ToLowerString(form.GetHost().ToString());
	form_data_.port_ = form.GetPort().ToString();
}

RequestTarget::RequestTarget(const AsteriskForm &form) : form_data_()
{
	(void)form;
	form_type_ = kAsteriskForm;
}

RequestTarget::RequestTarget(const RequestFormData &request_target) : form_data_(request_target) {}

RequestTarget &RequestTarget::operator=(const RequestTarget &other)
{
	if (this == &other) {
		return *this;
	}
	form_data_ = other.form_data_;
	return *this;
}

bool RequestTarget::operator==(const RequestTarget &rhs) const
{
	if (this == &rhs) {
		return true;
	}
	return form_data_ == rhs.form_data_;
}

bool RequestTarget::operator!=(const RequestTarget &rhs) const
{
	return !(*this == rhs);
}

const RequestFormData &RequestTarget::GetRequestFormData() const
{
	return form_data_;
}

std::ostream &operator<<(std::ostream &os, const RequestTarget &request_target)
{
	os << "\n" << request_target.GetRequestFormData();
	return os;
}
