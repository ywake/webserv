#include "gtest.h"

#include "webserv_utils.hpp"

typedef std::vector<std::string> Strings;

TEST(webserv_utils, join)
{
	EXPECT_EQ(utils::Join(Strings({"a", "b", "c"}), "/"), "a/b/c");
	EXPECT_EQ(utils::Join(Strings({"", "a", "b", "c"}), "/"), "/a/b/c");
	EXPECT_EQ(utils::Join(Strings({"", "a", "b", "c", ""}), "/"), "/a/b/c/");
}
