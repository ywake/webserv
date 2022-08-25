#include "gtest.h"

#include "ThinString.hpp"

#include <iostream>

TEST(thin_string, len)
{
	ThinString str;
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

TEST(thin_string, ToString)
{
	ThinString str;
	str = ThinString("0123ABC", 0, 20);
	EXPECT_EQ(str.ToString(), "0123ABC");
	str = ThinString("0123ABC", 0, 4);
	EXPECT_EQ(str.ToString(), "0123");
	str = ThinString("ABC0123ABC", 3, 4);
	EXPECT_EQ(str.ToString(), "0123");
	str = ThinString("ABC0123ABC", 7, 4);
	EXPECT_EQ(str.ToString(), "ABC");
}

TEST(thin_string, substr)
{
	ThinString str("0123456789", 3, 10);
	std::string act;
	act = str.substr(0, 5).ToString();
	EXPECT_EQ(act, "34567");
	act = str.substr(5, 5).ToString();
	EXPECT_EQ(act, "89");
	act = str.substr(10, 5).ToString();
	EXPECT_EQ(act, "");
}

TEST(thin_string, find)
{
	ThinString str("0123456789ABCDEF", 3, 10);
	// std::cout << str << std::endl;
	EXPECT_EQ(str.find("345"), 0);
}
