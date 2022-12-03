#include "asterisk_form.hpp"
#include "http_exceptions.hpp"

AsteriskForm::AsteriskForm(const ThinString &str)
{
	if (str != "*") {
		throw http::BadRequestException();
	}
}
