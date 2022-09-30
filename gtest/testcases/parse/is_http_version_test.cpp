#include "gtest.h"
#include "validate_request_line.hpp"

TEST(request_line, is_http_version_true_case)
{
	EXPECT_TRUE(http::abnf::IsHttpVersion("HTTP/1.1"));
	EXPECT_TRUE(http::abnf::IsHttpVersion("HTTP/1.2"));
	EXPECT_TRUE(http::abnf::IsHttpVersion("HTTP/1.0"));
	EXPECT_TRUE(http::abnf::IsHttpVersion("HTTP/0.9"));
	EXPECT_TRUE(http::abnf::IsHttpVersion("HTTP/4.2"));
}

TEST(request_line, is_http_version_false_case)
{
	EXPECT_FALSE(http::abnf::IsHttpVersion("HTTP/1.11"));
	EXPECT_FALSE(http::abnf::IsHttpVersion("HTTP/1.01"));
	EXPECT_FALSE(http::abnf::IsHttpVersion("HTTP/0.99"));
	EXPECT_FALSE(http::abnf::IsHttpVersion("HTTP/1.1 "));
	EXPECT_FALSE(http::abnf::IsHttpVersion(" HTTP/1.1"));
	EXPECT_FALSE(http::abnf::IsHttpVersion("HTTP/1.1\t"));
	EXPECT_FALSE(http::abnf::IsHttpVersion("\tHTTP/1.1"));
	EXPECT_FALSE(http::abnf::IsHttpVersion("HTTP/42.42"));
	EXPECT_FALSE(http::abnf::IsHttpVersion("HTTPS/1.1"));
	EXPECT_FALSE(http::abnf::IsHttpVersion("FTP/1.1"));
	EXPECT_FALSE(http::abnf::IsHttpVersion("HTTP 1.1"));
	EXPECT_FALSE(http::abnf::IsHttpVersion("HTTP/"));
	EXPECT_FALSE(http::abnf::IsHttpVersion("/1.1"));
	EXPECT_FALSE(http::abnf::IsHttpVersion("/"));
}
