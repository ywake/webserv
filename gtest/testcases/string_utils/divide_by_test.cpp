#include "gtest.h"

#include "webserv_utils.hpp"

TEST(string_utils, divide_by)
{
	std::string                         str     = "hello world";
	std::pair<std::string, std::string> divided = utils::DivideBy(str, ' ');
	EXPECT_EQ(divided.first, "hello");
	EXPECT_EQ(divided.second, "world");

	str     = "hello";
	divided = utils::DivideBy(str, ' ');
	EXPECT_EQ(divided.first, "hello");
	EXPECT_EQ(divided.second, "");

	str     = "helloworld ";
	divided = utils::DivideBy(str, ' ');
	EXPECT_EQ(divided.first, "helloworld");
	EXPECT_EQ(divided.second, "");

	str     = " helloworld";
	divided = utils::DivideBy(str, ' ');
	EXPECT_EQ(divided.first, "");
	EXPECT_EQ(divided.second, "helloworld");

	str     = " ";
	divided = utils::DivideBy(str, ' ');
	EXPECT_EQ(divided.first, "");
	EXPECT_EQ(divided.second, "");

	str     = "";
	divided = utils::DivideBy(str, ' ');
	EXPECT_EQ(divided.first, "");
	EXPECT_EQ(divided.second, "");
}
