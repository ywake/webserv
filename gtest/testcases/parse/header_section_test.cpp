#include "gtest.h"

#include <list>

#include "error.hpp"
#include "header_section.hpp"

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
	EXPECT_THROW(HeaderSection(":"), Error);
	EXPECT_THROW(HeaderSection(" :"), Error);
	EXPECT_THROW(HeaderSection("\r\n"), Error);
	EXPECT_THROW(HeaderSection("a:"), Error);
	EXPECT_THROW(
		HeaderSection("key: value\r\n"
					  "\r\n"
					  "\r\n"),
		Error
	);
	EXPECT_THROW(
		HeaderSection("key\r\n"
					  " : value\r\n"),
		Error
	);
	EXPECT_THROW(
		HeaderSection("key\r\n"
					  "\t: value\r\n"),
		Error
	);
	EXPECT_THROW(
		HeaderSection("key\r\n"
					  " : value"),
		Error
	);
	EXPECT_THROW(
		HeaderSection("key\r\n"
					  "\t: value"),
		Error
	);
	EXPECT_THROW(
		HeaderSection("key: value\r\n"
					  " \r\n"
					  "\t"),
		Error
	);
	EXPECT_THROW(
		HeaderSection("key: value\r\n"
					  " \r\n"
					  " "),
		Error
	);
}
