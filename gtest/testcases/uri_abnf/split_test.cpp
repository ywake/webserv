#include "gtest.h"
#include "uri_abnf.hpp"

TEST(uri_abnf, split_basic)
{
	ABNF::StringAry act = ABNF::Split("Hello World.", " ");
	ABNF::StringAry exp = ABNF::StringAry({"Hello", "World."});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, split_serial_delims)
{
	ABNF::StringAry act = ABNF::Split("Hello  World .", " ");
	ABNF::StringAry exp = ABNF::StringAry({"Hello", "World", "."});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, split_head)
{
	ABNF::StringAry act = ABNF::Split(" Hello  World .", " ");
	ABNF::StringAry exp = ABNF::StringAry({"Hello", "World", "."});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, split_tail)
{
	ABNF::StringAry act = ABNF::Split("Hello  World . ", " ");
	ABNF::StringAry exp = ABNF::StringAry({"Hello", "World", "."});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, split_2_len_delims)
{
	ABNF::StringAry act = ABNF::Split("Hello  World.", "  ");
	ABNF::StringAry exp = ABNF::StringAry({"Hello", "World."});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, split_len_delims)
{
	ABNF::StringAry act = ABNF::Split("Hello World.", "Hello");
	ABNF::StringAry exp = ABNF::StringAry({" World."});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, split_long_delims)
{
	ABNF::StringAry act = ABNF::Split("Hello", "              ");
	ABNF::StringAry exp = ABNF::StringAry({"Hello"});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, split_empty_delims)
{
	ABNF::StringAry act = ABNF::Split("Hello World.", "");
	ABNF::StringAry exp = ABNF::StringAry({"Hello World."});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, split_all_of_string_is_delim)
{
	ABNF::StringAry act = ABNF::Split("          ", " ");
	ABNF::StringAry exp = ABNF::StringAry({""});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, split_empty_x_empty)
{
	ABNF::StringAry act = ABNF::Split("", "");
	ABNF::StringAry exp = ABNF::StringAry({""});

	EXPECT_EQ(act, exp);
}
