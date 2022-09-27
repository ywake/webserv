#include "gtest.h"

#include <list>

#include "error.hpp"
#include "field_lines.hpp"

TEST(field_lines, test)
{
	FieldLines field_lines;

	EXPECT_EQ(
		FieldLines("key: value\r\n"
				   "key2: value2\r\n"
				   " value2\r\n"
				   "\tvalue2\r\n"),
		FieldLines(FieldLines::Headers(
			{{"key", FieldLines::Values("value")},
			 {"key2",
			  FieldLines::Values("value2\r\n"
								 " value2\r\n"
								 "\tvalue2")}}
		))
	);

	EXPECT_EQ(
		FieldLines("a:\r\n"), FieldLines(FieldLines::Headers({{"a", FieldLines::Values("")}}))
	);
	EXPECT_EQ(
		FieldLines("key: value\r\n"
				   "key: value2  \r\n"),
		FieldLines(FieldLines::Headers({{"key", FieldLines::Values("value, value2")}}))
	);
	EXPECT_EQ(
		FieldLines("key: value\r\n"
				   " \r\n"
				   "\t\r\n"),
		FieldLines(FieldLines::Headers(
			{{"key",
			  FieldLines::Values("value\r\n"
								 " \r\n"
								 "\t")}}
		))
	);
	EXPECT_EQ(
		FieldLines("key: value\r\n"
				   " \r\n"
				   "\t\r\n"),
		FieldLines(FieldLines::Headers({{"key", FieldLines::Values("value\r\n \r\n\t")}}))
	);
}

TEST(field_lines, throw_test)
{
	EXPECT_THROW(FieldLines(":"), Error);
	EXPECT_THROW(FieldLines(" :"), Error);
	EXPECT_THROW(FieldLines("\r\n"), Error);
	EXPECT_THROW(FieldLines("a:"), Error);
	EXPECT_THROW(
		FieldLines("key: value\r\n"
				   "\r\n"
				   "\r\n"),
		Error
	);
	EXPECT_THROW(
		FieldLines("key\r\n"
				   " : value\r\n"),
		Error
	);
	EXPECT_THROW(
		FieldLines("key\r\n"
				   "\t: value\r\n"),
		Error
	);
	EXPECT_THROW(
		FieldLines("key\r\n"
				   " : value"),
		Error
	);
	EXPECT_THROW(
		FieldLines("key\r\n"
				   "\t: value"),
		Error
	);
	EXPECT_THROW(
		FieldLines("key: value\r\n"
				   " \r\n"
				   "\t"),
		Error
	);
	EXPECT_THROW(
		FieldLines("key: value\r\n"
				   " \r\n"
				   " "),
		Error
	);
}
