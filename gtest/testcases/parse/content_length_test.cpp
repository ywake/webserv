#include "gtest.h"

#include "validate_headers.hpp"

#include "header_section.hpp"

TEST(http_headers, is_valid_content_length)
{
	EXPECT_TRUE(http::headers::IsValidContentLength("0"));
	EXPECT_TRUE(http::headers::IsValidContentLength("1"));
	EXPECT_TRUE(http::headers::IsValidContentLength("01"));
	EXPECT_TRUE(http::headers::IsValidContentLength("12345"));
	EXPECT_TRUE(http::headers::IsValidContentLength("9223372036854775807"));

	EXPECT_FALSE(http::headers::IsValidContentLength(""));
	EXPECT_FALSE(http::headers::IsValidContentLength("1, "));
	EXPECT_FALSE(http::headers::IsValidContentLength(", 1"));
	EXPECT_FALSE(http::headers::IsValidContentLength("1, 2"));
	EXPECT_FALSE(http::headers::IsValidContentLength("0, 0"));
	EXPECT_FALSE(http::headers::IsValidContentLength("a"));
	EXPECT_FALSE(http::headers::IsValidContentLength("0a"));
	EXPECT_FALSE(http::headers::IsValidContentLength("9223372036854775808"));
}

TEST(http_headers, is_valid_content_length_obs_fold)
{
	EXPECT_FALSE(http::headers::IsValidContentLength("\r\n 1"));
	EXPECT_FALSE(http::headers::IsValidContentLength("1\r\n "));
	EXPECT_FALSE(http::headers::IsValidContentLength("1\r\n\t"));
}
