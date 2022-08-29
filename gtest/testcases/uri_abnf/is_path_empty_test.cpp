#include "gtest.h"

#include "uri_abnf.hpp"

// host = IP-literal / IPv4address / reg-name
TEST(uri_abnf, is_path_empty_true)
{
	ThinString s = "0123456789";
	EXPECT_TRUE(ABNF::IsPathEmpty(""));
	EXPECT_TRUE(ABNF::IsPathEmpty(s.substr(0, 0)));
	EXPECT_TRUE(ABNF::IsPathEmpty(s.substr(10)));
}

TEST(uri_abnf, is_path_empty_false)
{
	EXPECT_FALSE(ABNF::IsPathEmpty("a"));
	ThinString s = "0123456789";
	EXPECT_FALSE(ABNF::IsPathEmpty(s.substr(9)));
	EXPECT_FALSE(ABNF::IsPathEmpty(s.substr(0, 1)));
}
