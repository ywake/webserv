#include "request_target.hpp"
#include "absolute_form.hpp"
#include "asterisk_form.hpp"
#include "authority_form.hpp"
#include "error.hpp"
#include "origin_form.hpp"
#include "parse_path.hpp"
#include "parse_uri.hpp"
#include "thin_string.hpp"

#include <string>
#include <vector>

RequestTarget::RequestTarget() : form_data_() {}

RequestTarget::RequestTarget(const ITargetForm &form)
{
	form_data_.scheme_	 = form.GetScheme();
	form_data_.userinfo_ = form.GetUserinfo();
	form_data_.host_	 = form.GetHost();
	form_data_.port_	 = form.GetPort();
	form_data_.path_	 = form.GetPath();
	form_data_.query_	 = form.GetQuery();
}

RequestTarget::RequestTarget(RequestFormData request_target) : form_data_(request_target) {}

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
