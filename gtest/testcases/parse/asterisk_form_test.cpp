#include "asterisk_form.hpp"
#include "error.hpp"
#include "gtest.h"

TEST(AsteriskFormTest, valid_asterisk_form)
{
	EXPECT_NO_THROW(AsteriskForm("*"));
}

TEST(AsteriskFormTest, invalid_asterisk_form)
{
	EXPECT_THROW(AsteriskForm(""), Error);
	EXPECT_THROW(AsteriskForm("index.html"), Error);
	EXPECT_THROW(AsteriskForm("index.html?query=1"), Error);
	EXPECT_THROW(AsteriskForm("?query=1"), Error);
	EXPECT_THROW(AsteriskForm("?"), Error);
}
