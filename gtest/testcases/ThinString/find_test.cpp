#include "gtest.h"

#include "ThinString.hpp"

#include <iostream>

TEST(thin_string, init)
{
	ThinString str("");
	EXPECT_EQ(str.len(), 0);
	EXPECT_EQ(str.size(), 0);
	EXPECT_EQ(str, "");
}

TEST(thin_string, len)
{
	ThinString str;
	EXPECT_EQ(str.len(), 0);
	str = ThinString("0123456789ABCDEF", 0, 16);
	EXPECT_EQ(str.len(), 16);
	str = ThinString("0123456789ABCDEF", 3, 10);
	EXPECT_EQ(str.len(), 10);
	str = ThinString("0123456789ABCDEF", 10, 10);
	EXPECT_EQ(str.len(), 6);
	str = ThinString("0123456789ABCDEF", 16, 10);
	EXPECT_EQ(str.len(), 0);
	str = ThinString("0123456789ABCDEF", 20, 10);
	EXPECT_EQ(str.len(), 0);
}

TEST(thin_string, size)
{
	ThinString str;
	EXPECT_EQ(str.size(), 0);
	str = ThinString("0123456789ABCDEF", 0, 16);
	EXPECT_EQ(str.size(), 16);
	str = ThinString("0123456789ABCDEF", 3, 10);
	EXPECT_EQ(str.size(), 10);
	str = ThinString("0123456789ABCDEF", 10, 10);
	EXPECT_EQ(str.size(), 6);
	str = ThinString("0123456789ABCDEF", 16, 10);
	EXPECT_EQ(str.size(), 0);
	str = ThinString("0123456789ABCDEF", 20, 10);
	EXPECT_EQ(str.size(), 0);
}

TEST(thin_string, ToString)
{
	ThinString str;
	EXPECT_EQ(str, "");
	str = ThinString("0123ABC", 0, 20);
	EXPECT_EQ(str, "0123ABC");
	str = ThinString("0123ABC", 0, 4);
	EXPECT_EQ(str, "0123");
	str = ThinString("ABC0123ABC", 3, 4);
	EXPECT_EQ(str, "0123");
	str = ThinString("ABC0123ABC", 7, 4);
	EXPECT_EQ(str, "ABC");
}

TEST(thin_string, substr)
{
	ThinString str("0123456789", 3, 10);
	ThinString act;
	act = str.substr(0, 5);
	EXPECT_EQ(act, "34567");
	act = str.substr(5, 5);
	EXPECT_EQ(act, "89");
	act = str.substr(10, 5);
	EXPECT_EQ(act, "");
}

TEST(thin_string, find)
{
	ThinString str("0123456789ABCDEF", 3, 10);
	// std::cout << str << std::endl;
	EXPECT_EQ(str.find("345"), 0);
}

TEST(thin_string, operator_equal)
{
	EXPECT_TRUE(ThinString("aaa") == ThinString("aaa"));
	EXPECT_TRUE(ThinString("aaa") == "aaa");
	EXPECT_TRUE(ThinString("aaa") == std::string("aaa"));

	EXPECT_TRUE(ThinString("abcdef", 0, 3) == ThinString("abc"));
	EXPECT_TRUE(ThinString("abcdef", 3, 3) == "def");
	EXPECT_TRUE(ThinString("abcdef", 3, 10) == std::string("def"));

	EXPECT_FALSE(ThinString("aaa") == ThinString(""));
	EXPECT_FALSE(ThinString("aaa") == "");
	EXPECT_FALSE(ThinString("aaa") == std::string(""));

	EXPECT_FALSE(ThinString("") == ThinString("bbb"));
	EXPECT_FALSE(ThinString("") == "bbb");
	EXPECT_FALSE(ThinString("") == std::string("bbb"));

	EXPECT_FALSE(ThinString("bb") == ThinString("bbb"));
	EXPECT_FALSE(ThinString("bb") == "bbb");
	EXPECT_FALSE(ThinString("bb") == std::string("bbb"));

	EXPECT_FALSE(ThinString("aaa") == ThinString("bbb"));
	EXPECT_FALSE(ThinString("aaa") == "bbb");
	EXPECT_FALSE(ThinString("aaa") == std::string("bbb"));

	EXPECT_FALSE(ThinString("abcdef") == ThinString("abcdef", 0, 4));
	EXPECT_FALSE(ThinString("abcdef") == "abcd");
	EXPECT_FALSE(ThinString("abcdef") == std::string("abcd"));
}

TEST(thin_string, DivideBy)
{
	ThinString str("aaa/bbb");
	ThinString::ThinStrPair p = str.DivideBy("/");
	EXPECT_EQ(p.first, "aaa");
	EXPECT_EQ(p.second, "bbb");
}
