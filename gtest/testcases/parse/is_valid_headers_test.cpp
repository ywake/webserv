#include "gtest.h"

#include "http_exceptions.hpp"
#include "validate_headers.hpp"

TEST(validate_headers, must_have_a_host)
{
	EXPECT_NO_THROW(http::headers::ValidateHeaderSection(HeaderSection("Host: localhost\r\n")));

	EXPECT_NO_THROW(http::headers::ValidateHeaderSection(HeaderSection("key: value \r\n"
																	   "Host: localhost\r\n")));

	EXPECT_THROW(
		http::headers::ValidateHeaderSection(HeaderSection("key: value\r\n")),
		http::BadRequestException
	);

	EXPECT_THROW(
		http::headers::ValidateHeaderSection(HeaderSection("key: value\r\n"
														   "Host: localhost\r\n"
														   "Host: localhost\r\n")),
		http::BadRequestException
	);

	EXPECT_THROW(
		http::headers::ValidateHeaderSection(HeaderSection("Host: localhost\r\n"
														   "Host: localhost\r\n"
														   "Host: localhost\r\n")),
		http::BadRequestException
	);
}

TEST(validate_headers, must_not_have_both)
{
	EXPECT_NO_THROW(http::headers::ValidateHeaderSection(HeaderSection("Host: localhost\r\n"
																	   "Content-Length: 0\r\n")));

	EXPECT_NO_THROW(
		http::headers::ValidateHeaderSection(HeaderSection("Host: localhost\r\n"
														   "Transfer-Encoding: chunked\r\n"))
	);

	EXPECT_THROW(
		http::headers::ValidateHeaderSection(HeaderSection("Host: localhost\r\n"
														   "Content-Length: 0\r\n"
														   "Transfer-Encoding: chunked\r\n")),
		http::BadRequestException
	);

	EXPECT_THROW(
		http::headers::ValidateHeaderSection(HeaderSection("Host: localhost\r\n"
														   "Transfer-Encoding: chunked\r\n"
														   "Content-Length: 0\r\n")),
		http::BadRequestException
	);
}
