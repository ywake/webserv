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

TEST(http_utils, parse_list)
{
	EXPECT_EQ(http::ParseList("abc,def"), std::vector<ThinString>({"abc", "def"}));
	EXPECT_EQ(http::ParseList("abc, def"), std::vector<ThinString>({"abc", "def"}));
	EXPECT_EQ(http::ParseList("abc , def"), std::vector<ThinString>({"abc", "def"}));
	EXPECT_EQ(http::ParseList("abc ,def"), std::vector<ThinString>({"abc", "def"}));
	EXPECT_EQ(http::ParseList("abc, def, ghi"), std::vector<ThinString>({"abc", "def", "ghi"}));
	EXPECT_EQ(
		http::ParseList("abc, \"def, ghi\""), std::vector<ThinString>({"abc", "\"def", "ghi\""})
	);
	// ignore_quote = false
	EXPECT_EQ(
		http::ParseList("abc, def, ghi", false), std::vector<ThinString>({"abc", "def", "ghi"})
	);
	EXPECT_EQ(http::ParseList("abc, def,", false), std::vector<ThinString>({"abc", "def", ""}));
	EXPECT_EQ(http::ParseList(",abc, def", false), std::vector<ThinString>({"", "abc", "def"}));
	EXPECT_EQ(
		http::ParseList("abc, \"def, ghi\"", false),
		std::vector<ThinString>({"abc", "\"def, ghi\""})
	);
}
