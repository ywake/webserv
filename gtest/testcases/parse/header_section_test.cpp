#include "gtest.h"

#include <list>

#include "header_section.hpp"
#include "http_exceptions.hpp"

TEST(field_lines, test)
{
	HeaderSection field_lines;

	EXPECT_EQ(
		HeaderSection("key: value\r\n"
					  "key2: value2\r\n"
					  " value2\r\n"
					  "\tvalue2\r\n"),
		HeaderSection(HeaderSection::Headers(
			{{"key", "value"},
			 {"key2",
			  "value2\r\n"
			  " value2\r\n"
			  "\tvalue2"}}
		))
	);

	EXPECT_EQ(HeaderSection("a:\r\n"), HeaderSection(HeaderSection::Headers({{"a", ""}})));
	EXPECT_EQ(
		HeaderSection("key: value\r\n"
					  "key: value2  \r\n"),
		HeaderSection(HeaderSection::Headers({{"key", "value, value2"}}))
	);
	EXPECT_EQ(
		HeaderSection("key: value\r\n"
					  " \r\n"
					  "\t\r\n"),
		HeaderSection(HeaderSection::Headers(
			{{"key",
			  "value\r\n"
			  " \r\n"
			  "\t"}}
		))
	);
	EXPECT_EQ(
		HeaderSection("key: value\r\n"
					  " \r\n"
					  "\t\r\n"),
		HeaderSection(HeaderSection::Headers({{"key", "value\r\n \r\n\t"}}))
	);
}

TEST(field_lines, throw_test)
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
