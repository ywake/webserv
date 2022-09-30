#include "gtest.h"

#include <list>

#include "header_section.hpp"
#include "http_exceptions.hpp"

TEST(header_section, general)
{
	EXPECT_EQ(
		HeaderSection("key: value\r\n"
					  "key2: value2\r\n"
					  " value2\r\n"
					  "\tvalue2\r\n"),
		HeaderSection(HeaderSection::Headers(
			{{"key", {HeaderValue("value")}},
			 {"key2",
			  {HeaderValue("value2\r\n"
						   " value2\r\n"
						   "\tvalue2")}}}
		))
	);

	EXPECT_EQ(
		HeaderSection("a:\r\n"), HeaderSection(HeaderSection::Headers({{"a", {HeaderValue("")}}}))
	);
	EXPECT_EQ(
		HeaderSection("key: value\r\n"
					  "key: value2  \r\n"),
		HeaderSection(
			HeaderSection::Headers({{"key", {HeaderValue("value"), HeaderValue("value2")}}})
		)
	);
	EXPECT_EQ(
		HeaderSection("key: value\r\n"
					  " \r\n"
					  "\t\r\n"),
		HeaderSection(HeaderSection::Headers(
			{{"key",
			  {HeaderValue("value\r\n"
						   " \r\n"
						   "\t")}}}
		))
	);
	EXPECT_EQ(
		HeaderSection("key: value\r\n"
					  " \r\n"
					  "\t\r\n"),
		HeaderSection(HeaderSection::Headers({{"key", {HeaderValue("value\r\n \r\n\t")}}}))
	);
}

TEST(header_section, throw_test)
{
	EXPECT_THROW(HeaderSection(":"), http::BadRequestException);
	EXPECT_THROW(HeaderSection(" :"), http::BadRequestException);
	EXPECT_THROW(HeaderSection("\r\n"), http::BadRequestException);
	EXPECT_THROW(HeaderSection("a:"), http::BadRequestException);
	EXPECT_THROW(
		HeaderSection("key: value\r\n"
					  "\r\n"
					  "\r\n"),
		http::BadRequestException
	);
	EXPECT_THROW(
		HeaderSection("key\r\n"
					  " : value\r\n"),
		http::BadRequestException
	);
	EXPECT_THROW(
		HeaderSection("key\r\n"
					  "\t: value\r\n"),
		http::BadRequestException
	);
	EXPECT_THROW(
		HeaderSection("key\r\n"
					  " : value"),
		http::BadRequestException
	);
	EXPECT_THROW(
		HeaderSection("key\r\n"
					  "\t: value"),
		http::BadRequestException
	);
	EXPECT_THROW(
		HeaderSection("key: value\r\n"
					  " \r\n"
					  "\t"),
		http::BadRequestException
	);
	EXPECT_THROW(
		HeaderSection("key: value\r\n"
					  " \r\n"
					  " "),
		http::BadRequestException
	);
}

TEST(header_section, content_length)
{
	EXPECT_EQ(
		HeaderSection("content-length:11\r\n"),
		HeaderSection(HeaderSection::Headers({{"content-length", {HeaderValue("11")}}}))
	);
	EXPECT_EQ(
		HeaderSection("Content-Length:11\r\n"),
		HeaderSection(HeaderSection::Headers({{"content-length", {HeaderValue("11")}}}))
	);

	// error case
	EXPECT_THROW(HeaderSection("Content-Length:\r\n"), http::BadRequestException);
}
