#include "gtest.h"

#include "uri_abnf.hpp"

// host = IP-literal / IPv4address / reg-name
TEST(uri_abnf, is_host_ipv6)
{
	EXPECT_TRUE(ABNF::IsHost("[::1]"));
}

TEST(uri_abnf, is_host_ipv4)
{
	EXPECT_TRUE(ABNF::IsHost("127.0.0.1"));
}

TEST(uri_abnf, is_host_regname_true)
{
	EXPECT_TRUE(ABNF::IsHost("localhost"));
}

TEST(uri_abnf, is_host_regname_false)
{
	EXPECT_FALSE(ABNF::IsHost("")); // HTTP要件
}
