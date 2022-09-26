#include "authority_form.hpp"
#include "error.hpp"

static const ThinString kEmpty = "";

AuthorityForm::AuthorityForm() : authority_() {}

// authority-form = uri-host ":" port
// [RFC3986] the "http" scheme considers a missing authority or empty host invalid.
AuthorityForm::AuthorityForm(const ThinString &str) : authority_(str)
{
	if (authority_.GetHost().empty() || str.find(":") == ThinString::npos) {
		throw Error("400");
	}
}

AuthorityForm::AuthorityForm(const AuthorityForm &other) : authority_(other.authority_) {}

AuthorityForm::AuthorityForm(const Authority &authority) : authority_(authority) {}

AuthorityForm &AuthorityForm::operator=(const AuthorityForm &other)
{
	if (this == &other) {
		return *this;
	}
	authority_ = other.authority_;
	return *this;
}

bool AuthorityForm::operator==(const AuthorityForm &other) const
{
	if (this == &other) {
		return true;
	}
	return authority_ == other.authority_;
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
	return authority_.GetHost();
}

const ThinString &AuthorityForm::GetPort() const
{
	return authority_.GetPort();
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
