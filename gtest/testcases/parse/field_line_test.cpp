#include "gtest.h"

#include "field_line.hpp"
#include "http_exceptions.hpp"

TEST(filed_line, field_line_test)
{
	EXPECT_EQ(FieldLine("Content-Length: 11"), FieldLine("Content-Length", "11"));
	// field-name should be a token
	EXPECT_EQ(FieldLine("Content-Length: 11"), FieldLine("Content-Length", "11"));
}

/**
 *  パースと大小文字の区別を分離するために、フィールドラインクラス上では大小文字を区別しておく
 */
TEST(filed_line, field_line_is_case_sensitive)
{
	EXPECT_EQ(FieldLine("Content-Length: 11"), FieldLine("Content-Length", "11"));
	EXPECT_NE(FieldLine("Content-Length: 11"), FieldLine("content-length", "11"));
}

TEST(field_line, field_name_should_be_a_token)
{
	EXPECT_EQ(FieldLine("!#$%&'*+-.^_`|~: 11"), FieldLine("!#$%&'*+-.^_`|~", "11"));
	EXPECT_EQ(
		FieldLine("abcdefghijklmnopqrstuvwxyz: 11"), FieldLine("abcdefghijklmnopqrstuvwxyz", "11")
	);
	EXPECT_EQ(
		FieldLine("ABCDEFGHIJKLMNOPQRSTUVWXYZ: 11"), FieldLine("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "11")
	);
	EXPECT_EQ(FieldLine("1234567890: 11"), FieldLine("1234567890", "11"));

	EXPECT_THROW(FieldLine("@: 11"), ParseErrorException);
	EXPECT_THROW(FieldLine("[]: 11"), ParseErrorException);
	EXPECT_THROW(FieldLine(":aaa: 11"), ParseErrorException);
}

TEST(filed_line, field_value_is_field_content)
{
	EXPECT_EQ(FieldLine("aaa: "), FieldLine("aaa", ""));
	EXPECT_EQ(FieldLine("aaa: bbb"), FieldLine("aaa", "bbb"));
	EXPECT_EQ(FieldLine("aaa: 	a b	c"), FieldLine("aaa", "a b	c"));
	EXPECT_EQ(FieldLine("aaa:a b	c 	"), FieldLine("aaa", "a b	c"));
}

TEST(field_line, field_value_has_obsfold)
{
	EXPECT_EQ(FieldLine("aaa:\r\n "), FieldLine("aaa", "\r\n "));
	EXPECT_EQ(FieldLine("aaa: \r\n bbb"), FieldLine("aaa", "\r\n bbb"));
	EXPECT_EQ(FieldLine("aaa:bbb \r\n bbb"), FieldLine("aaa", "bbb \r\n bbb"));
	EXPECT_EQ(FieldLine("aaa: \r\n bbb\r\n "), FieldLine("aaa", "\r\n bbb\r\n "));
	EXPECT_EQ(FieldLine("aaa: \r\n bbb\r\n ccc"), FieldLine("aaa", "\r\n bbb\r\n ccc"));
}
