#include "gtest.h"

#include "parse_http_utils.hpp"

TEST(http_utils, trim_ows)
{
	EXPECT_EQ(http::TrimOws(" abc  "), "abc");
	EXPECT_EQ(http::TrimOws(" \tabc"), "abc");
	EXPECT_EQ(http::TrimOws("abc\t "), "abc");
	EXPECT_EQ(http::TrimOws("abc"), "abc");
	EXPECT_EQ(http::TrimOws("  "), "");
	EXPECT_EQ(http::TrimOws("\t\t"), "");
	EXPECT_EQ(http::TrimOws(""), "");
}

// [TODO] 一旦クオート未対応で進める。要相談。
TEST(http_utils, parse_list)
{
	EXPECT_EQ(http::ParseList("abc,def"), std::vector<ThinString>({"abc", "def"}));
	EXPECT_EQ(http::ParseList("abc, def"), std::vector<ThinString>({"abc", "def"}));
	EXPECT_EQ(http::ParseList("abc , def"), std::vector<ThinString>({"abc", "def"}));
	EXPECT_EQ(http::ParseList("abc ,def"), std::vector<ThinString>({"abc", "def"}));
	EXPECT_EQ(http::ParseList("abc, def, ghi"), std::vector<ThinString>({"abc", "def", "ghi"}));
	//EXPECT_EQ(
	//	http::ParseList("abc, \"def, ghi\""), std::vector<ThinString>({"abc", "\"def", "ghi\""})
	//);
	// ignore_quote = false
	EXPECT_EQ(
		http::ParseList("abc, def, ghi", false), std::vector<ThinString>({"abc", "def", "ghi"})
	);
	EXPECT_EQ(http::ParseList("abc, def,", false), std::vector<ThinString>({"abc", "def", ""}));
	EXPECT_EQ(http::ParseList(",abc, def", false), std::vector<ThinString>({"", "abc", "def"}));
	//EXPECT_EQ(
	//	http::ParseList("abc, \"def, ghi\"", false),
	//	std::vector<ThinString>({"abc", "\"def, ghi\""})
	//);

	EXPECT_EQ(http::ParseList(" abc "), std::vector<ThinString>({" abc "}));
	EXPECT_EQ(http::ParseList(" abc"), std::vector<ThinString>({" abc"}));
	EXPECT_EQ(http::ParseList("abc "), std::vector<ThinString>({"abc "}));
	EXPECT_EQ(http::ParseList(" abc , def "), std::vector<ThinString>({" abc", "def "}));
	EXPECT_EQ(http::ParseList(" abc , def , ghi "), std::vector<ThinString>({" abc", "def", "ghi "}));
	EXPECT_EQ(http::ParseList(" abc , def , ghi , jkl , mno "), std::vector<ThinString>({" abc", "def", "ghi", "jkl", "mno "}));
}
