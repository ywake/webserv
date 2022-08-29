/*
#include "gtest.h"

#include "uri_abnf.hpp"

TEST(uri_abnf, is_host_ipv6)
{
	EXPECT_TRUE(ABNF::IsHost("[::1]"));
}

TEST(uri_abnf, is_host_regname_true)
{
	EXPECT_TRUE(ABNF::IsHost("localhost"));
}

TEST(uri_abnf, is_host_regname_false)
{
	EXPECT_FALSE(ABNF::IsHost(""));
}
*/