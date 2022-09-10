#include "absolute_form.hpp"
#include "error.hpp"

// absolute-form = absolute-URI
// http-URI = "http" "://" authority path-abempty [ "?" query ]
// [RFC 9112 4.2.1] A sender MUST NOT generate an "http" URI with an empty host identifier. A
// recipient that processes such a URI reference MUST reject it as invalid.
AbsoluteForm::AbsoluteForm(const ThinString request_target) : uri_(request_target)
{
	bool is_pathrootless = !GetPath().empty() && GetPath().at(0) != '/';
	if (is_pathrootless || GetScheme() != "http" || GetHost().empty()) {
		throw Error("400");
	}
}

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
	return uri_.GetScheme();
}
