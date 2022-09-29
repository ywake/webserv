#include "asterisk_form.hpp"
#include "gtest.h"
#include "http_exceptions.hpp"

TEST(asterisk_form_test, valid_asterisk_form)
{
	EXPECT_NO_THROW(AsteriskForm("*"));
}

TEST(asterisk_form_test, invalid_asterisk_form)
{
	EXPECT_THROW(AsteriskForm(""), BadRequestException);
	EXPECT_THROW(AsteriskForm("index.html"), BadRequestException);
	EXPECT_THROW(AsteriskForm("index.html?query=1"), BadRequestException);
	EXPECT_THROW(AsteriskForm("?query=1"), BadRequestException);
	EXPECT_THROW(AsteriskForm("?"), BadRequestException);
}
