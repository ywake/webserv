#include "asterisk_form.hpp"
#include "http_exceptions.hpp"

static const ThinString kEmpty = "";

AsteriskForm::AsteriskForm(const ThinString &str)
{
	if (str != "*") {
		throw ParseErrorException();
	}
	asterisk_ = str;
}

const ThinString &AsteriskForm::GetScheme() const
{
	return kEmpty;
}

const ThinString &AsteriskForm::GetUserinfo() const
{
	return kEmpty;
}

const ThinString &AsteriskForm::GetHost() const
{
	return kEmpty;
}

const ThinString &AsteriskForm::GetPort() const
{
	return kEmpty;
}

const ThinString &AsteriskForm::GetPath() const
{
	return asterisk_;
}

const ThinString &AsteriskForm::GetQuery() const
{
	return kEmpty;
}
