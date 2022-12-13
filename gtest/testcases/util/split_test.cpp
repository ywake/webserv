#include "gtest.h"

#include "webserv_utils.hpp"

TEST(webserv_utils, split)
{
	EXPECT_EQ(utils::Split("a b c", " "), std::vector<std::string>({"a", "b", "c"}));
	EXPECT_EQ(utils::Split("/a/b/c", "/"), std::vector<std::string>({"", "a", "b", "c"}));
	EXPECT_EQ(utils::Split("/a/b/c/", "/"), std::vector<std::string>({"", "a", "b", "c", ""}));
}
