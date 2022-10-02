#include "gtest.h"

#include "validate_headers.hpp"

TEST(validate_headers, must_have_a_host)
{
	EXPECT_TRUE(http::headers::IsValidHeaderSection(HeaderSection("Host: localhost\r\n")));

	EXPECT_TRUE(http::headers::IsValidHeaderSection(HeaderSection("key: value \r\n"
																  "Host: localhost\r\n")));

	EXPECT_FALSE(http::headers::IsValidHeaderSection(HeaderSection("key: value\r\n")));

	EXPECT_FALSE(http::headers::IsValidHeaderSection(HeaderSection("key: value\r\n"
																   "Host: localhost\r\n"
																   "Host: localhost\r\n")));

	EXPECT_FALSE(http::headers::IsValidHeaderSection(HeaderSection("Host: localhost\r\n"
																   "Host: localhost\r\n"
																   "Host: localhost\r\n")));
}
