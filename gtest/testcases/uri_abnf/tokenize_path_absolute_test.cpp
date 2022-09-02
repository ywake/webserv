#include "gtest.h"
#include "parse_path_utils.hpp"

TEST(uri_abnf, tokenize_path_absolute_basic)
{
	ABNF::StringAry act = ABNF::TokenizePath("aaa/bbb");
	ABNF::StringAry exp = ABNF::StringAry({"aaa", "/", "bbb"});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, tokenize_path_absolute_serial_delims)
{
	ABNF::StringAry act = ABNF::TokenizePath("aaa///bbb");
	ABNF::StringAry exp = ABNF::StringAry({"aaa", "/", "/", "/", "bbb"});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, tokenize_path_absolute_head)
{
	ABNF::StringAry act = ABNF::TokenizePath("/aaa/bbb");
	ABNF::StringAry exp = ABNF::StringAry({"/", "aaa", "/", "bbb"});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, tokenize_path_absolute_tail)
{
	ABNF::StringAry act = ABNF::TokenizePath("aaa/bbb/");
	ABNF::StringAry exp = ABNF::StringAry({"aaa", "/", "bbb", "/"});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, tokenize_path_absolute_all_of_string_is_delim)
{
	ABNF::StringAry act = ABNF::TokenizePath("////");
	ABNF::StringAry exp = ABNF::StringAry({"/", "/", "/", "/"});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, tokenize_path_absolute_empty_x_empty)
{
	ABNF::StringAry act = ABNF::TokenizePath("");
	ABNF::StringAry exp = ABNF::StringAry({});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, tokenize_path_absolute_slash_only)
{
	ABNF::StringAry act = ABNF::TokenizePath("/");
	ABNF::StringAry exp = ABNF::StringAry({"/"});

	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, tokenize_path_absolute_slash_a)
{
	ABNF::StringAry act = ABNF::TokenizePath("/a");
	ABNF::StringAry exp = ABNF::StringAry({"/", "a"});

	EXPECT_EQ(act, exp);
}
