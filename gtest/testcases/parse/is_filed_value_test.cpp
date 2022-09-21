#include "gtest.h"

#include "validate_field_line.hpp"

TEST(field_line, is_field_value_true)
{
	EXPECT_TRUE(http_abnf::IsFieldValue(""));
	EXPECT_TRUE(http_abnf::IsFieldValue("a"));
	EXPECT_TRUE(http_abnf::IsFieldValue("a a"));
	EXPECT_TRUE(http_abnf::IsFieldValue("a	a"));
	EXPECT_TRUE(http_abnf::IsFieldValue("a\r\n "));
	EXPECT_TRUE(http_abnf::IsFieldValue("\r\n a"));
	EXPECT_TRUE(http_abnf::IsFieldValue("\r\n "));
	EXPECT_TRUE(http_abnf::IsFieldValue("\r\n	"));
	EXPECT_TRUE(http_abnf::IsFieldValue("\r\n 	 	"));
	EXPECT_TRUE(http_abnf::IsFieldValue("a\r\n      "));
	EXPECT_TRUE(http_abnf::IsFieldValue("\r\n       "));

	EXPECT_TRUE(http_abnf::IsFieldValue(""));
	EXPECT_TRUE(http_abnf::IsFieldValue("\xff"));
	EXPECT_TRUE(http_abnf::IsFieldValue("\xff \xff"));
	EXPECT_TRUE(http_abnf::IsFieldValue("\xff	\xff"));
	EXPECT_TRUE(http_abnf::IsFieldValue("\xff\r\n "));
	EXPECT_TRUE(http_abnf::IsFieldValue("\r\n \xff"));
	EXPECT_TRUE(http_abnf::IsFieldValue("\r\n "));
	EXPECT_TRUE(http_abnf::IsFieldValue("\r\n	"));
	EXPECT_TRUE(http_abnf::IsFieldValue("\r\n 	 	"));
	EXPECT_TRUE(http_abnf::IsFieldValue("\xff\r\n      "));
	EXPECT_TRUE(http_abnf::IsFieldValue("\r\n       "));
}

TEST(field_line, is_field_value_false)
{
	EXPECT_FALSE(http_abnf::IsFieldValue(" "));
	EXPECT_FALSE(http_abnf::IsFieldValue("	"));
	EXPECT_FALSE(http_abnf::IsFieldValue("\n"));
	EXPECT_FALSE(http_abnf::IsFieldValue("\r\n"));
	EXPECT_FALSE(http_abnf::IsFieldValue("a "));
	EXPECT_FALSE(http_abnf::IsFieldValue("a	"));
	EXPECT_FALSE(http_abnf::IsFieldValue(" a"));
	EXPECT_FALSE(http_abnf::IsFieldValue("	a"));
	EXPECT_FALSE(http_abnf::IsFieldValue("a\r\na"));
	EXPECT_FALSE(http_abnf::IsFieldValue("a\ra"));
	EXPECT_FALSE(http_abnf::IsFieldValue("a\na"));

	EXPECT_FALSE(http_abnf::IsFieldValue(" "));
	EXPECT_FALSE(http_abnf::IsFieldValue("	"));
	EXPECT_FALSE(http_abnf::IsFieldValue("\n"));
	EXPECT_FALSE(http_abnf::IsFieldValue("\r\n"));
	EXPECT_FALSE(http_abnf::IsFieldValue("\xff "));
	EXPECT_FALSE(http_abnf::IsFieldValue("\xff	"));
	EXPECT_FALSE(http_abnf::IsFieldValue(" \xff"));
	EXPECT_FALSE(http_abnf::IsFieldValue("	\xff"));
	EXPECT_FALSE(http_abnf::IsFieldValue("\xff\r\n\xff"));
	EXPECT_FALSE(http_abnf::IsFieldValue("\xff\r\xff"));
	EXPECT_FALSE(http_abnf::IsFieldValue("\xff\n\xff"));
}
