#include "absolute_form.hpp"
#include "error.hpp"

// absolute-form = absolute-URI
// http-URI = "http" "://" authority path-abempty [ "?" query ]
// [RFC 9112 4.2.1] A sender MUST NOT generate an "http" URI with an empty host identifier. A
// recipient that processes such a URI reference MUST reject it as invalid.
AbsoluteForm::AbsoluteForm(const ThinString request_target) : uri_(request_target)
{
	bool is_pathrootless = !GetPath().empty() && GetPath().at(0) != '/';
	bool is_http_family  = GetScheme() == "http" || GetScheme() == "https";
	if (is_pathrootless || !is_http_family || GetHost().empty()) {
		throw Error("400");
	}
}

AbsoluteForm::AbsoluteForm(const AbsoluteUri uri) : uri_(uri) {}

const ThinString &AbsoluteForm::GetScheme() const
{
	return uri_.GetScheme();
}

const ThinString &AbsoluteForm::GetUserinfo() const
{
	return uri_.GetHierPart().GetAuthority().GetUserinfo();
}

const ThinString &AbsoluteForm::GetHost() const
{
	return uri_.GetHierPart().GetAuthority().GetHost();
}

const ThinString &AbsoluteForm::GetPort() const
{
	return uri_.GetHierPart().GetAuthority().GetPort();
}

const ThinString &AbsoluteForm::GetPath() const
{
	return uri_.GetHierPart().GetPath();
}

const ThinString &AbsoluteForm::GetQuery() const
{
	return uri_.GetQuery();
}

bool AbsoluteForm::operator==(const AbsoluteForm &rhs) const
{
	if (this == &rhs) {
		return true;
	}
	return uri_ == rhs.GetUri();
}

bool AbsoluteForm::operator!=(const AbsoluteForm &rhs) const
{
	return !(rhs == *this);
}

const AbsoluteUri &AbsoluteForm::GetUri() const
{
	return uri_;
}

std::ostream &operator<<(std::ostream &os, const AbsoluteForm &form)
{
	os << form.GetUri();
	return os;
}
