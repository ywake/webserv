#include "gtest.h"

#include "uri_abnf.hpp"

TEST(uri_abnf, is_ip_literal_test)
{
	EXPECT_TRUE(ABNF::IsIPLiteral("[::1]"));
	EXPECT_TRUE(ABNF::IsIPLiteral("[v8.x:x:x:x]"));

	EXPECT_FALSE(ABNF::IsIPLiteral(""));
	EXPECT_FALSE(ABNF::IsIPLiteral("::1"));
	EXPECT_FALSE(ABNF::IsIPLiteral("v8.x:x:x:x"));
}
