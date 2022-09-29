#include "authority_form.hpp"
#include "error.hpp"

static const ThinString kEmpty = "";

AuthorityForm::AuthorityForm() : host_port_() {}

// authority-form = uri-host ":" port
// [RFC3986] the "http" scheme considers a missing authority or empty host invalid.
AuthorityForm::AuthorityForm(const ThinString &str) : host_port_(str)
{
	if (host_port_.GetHost().empty() || str.find(":") == ThinString::npos) {
		throw Error("400");
	}
}

AuthorityForm::AuthorityForm(const AuthorityForm &other) : host_port_(other.host_port_) {}

AuthorityForm::AuthorityForm(const http_abnf::HostPort &host_port) : host_port_(host_port) {}

AuthorityForm &AuthorityForm::operator=(const AuthorityForm &other)
{
	if (this == &other) {
		return *this;
	}
	host_port_ = other.host_port_;
	return *this;
}

bool AuthorityForm::operator==(const AuthorityForm &other) const
{
	if (this == &other) {
		return true;
	}
	return host_port_ == other.host_port_;
}

bool AuthorityForm::operator!=(const AuthorityForm &other) const
{
	return !(*this == other);
}

const ThinString &AuthorityForm::GetScheme() const
{
	return kEmpty;
}

const ThinString &AuthorityForm::GetUserinfo() const
{
	return kEmpty;
}

const ThinString &AuthorityForm::GetHost() const
{
	return host_port_.GetHost();
}

const ThinString &AuthorityForm::GetPort() const
{
	return host_port_.GetPort();
}

const ThinString &AuthorityForm::GetPath() const
{
	return kEmpty;
}

const ThinString &AuthorityForm::GetQuery() const
{
	return kEmpty;
}

std::ostream &operator<<(std::ostream &os, const AuthorityForm &authority)
{
	os << authority.GetHost() << std::endl;
	os << authority.GetPort() << std::endl;
	return os;
}
