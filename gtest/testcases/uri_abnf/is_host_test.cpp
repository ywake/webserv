#include "gtest.h"

#include "parse_authority.hpp"

// host = IP-literal / IPv4address / reg-name
TEST(uri_abnf, is_host_ip_literal)
{
	EXPECT_TRUE(ABNF::IsHost("[::1]"));
	EXPECT_TRUE(ABNF::IsHost("[vA.abcdefg]"));

	EXPECT_FALSE(ABNF::IsHost("[127.0.0.1]"));
	EXPECT_FALSE(ABNF::IsHost("[::1"));
	EXPECT_FALSE(ABNF::IsHost("::1]"));
	EXPECT_FALSE(ABNF::IsHost("[vA.abcdefg"));
	EXPECT_FALSE(ABNF::IsHost("vA.abcdefg]"));
}

TEST(uri_abnf, is_host_ipv4)
{
	EXPECT_TRUE(ABNF::IsHost("127.0.0.1"));

	EXPECT_FALSE(ABNF::IsHost("::1"));
}

TEST(uri_abnf, is_host_regname)
{
	EXPECT_TRUE(ABNF::IsHost("localhost"));
	EXPECT_TRUE(ABNF::IsHost("127.0.01"));

	EXPECT_FALSE(ABNF::IsHost("aiu@eo"));
	EXPECT_FALSE(ABNF::IsHost("")); // HTTP要件
}
