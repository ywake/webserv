#include "gtest.h"

#include "parse_authority.hpp"

// authority = [ userinfo "@" ] host [ ":" port ]
TEST(uri_abnf, is_authority_userinfo)
{
	EXPECT_TRUE(ABNF::IsAuthority("@"));
	EXPECT_TRUE(ABNF::IsAuthority("username@"));
	EXPECT_TRUE(ABNF::IsAuthority("username:pass@"));
	EXPECT_TRUE(ABNF::IsAuthority("username:pass:@"));
	EXPECT_TRUE(ABNF::IsAuthority("username::@"));
	EXPECT_TRUE(ABNF::IsAuthority("username:pass:xxx@"));

	EXPECT_FALSE(ABNF::IsAuthority("username:pass:xxx"));
}

TEST(uri_abnf, is_authority_host)
{
	EXPECT_TRUE(ABNF::IsAuthority(""));
	EXPECT_TRUE(ABNF::IsAuthority("example.com"));

	EXPECT_FALSE(ABNF::IsAuthority("example@com"));
}

TEST(uri_abnf, is_authority_port)
{
	EXPECT_TRUE(ABNF::IsAuthority(":"));
	EXPECT_TRUE(ABNF::IsAuthority(":80"));

	EXPECT_TRUE(ABNF::IsAuthority(""));
	EXPECT_FALSE(ABNF::IsAuthority(":aaa"));
}

TEST(uri_abnf, is_authority_userinfo_host)
{
	EXPECT_TRUE(ABNF::IsAuthority("username@host"));
	EXPECT_TRUE(ABNF::IsAuthority("username:pass@host"));
	EXPECT_TRUE(ABNF::IsAuthority("username:pass:@host"));
	EXPECT_TRUE(ABNF::IsAuthority("username:pass:xxx@host"));
	EXPECT_TRUE(ABNF::IsAuthority("username::@host"));
}

TEST(uri_abnf, is_authority_host_port)
{
	EXPECT_TRUE(ABNF::IsAuthority("host:80"));
	EXPECT_FALSE(ABNF::IsAuthority("host:aaa"));
}

TEST(uri_abnf, is_authority_userinfo_port)
{
	EXPECT_TRUE(ABNF::IsAuthority("username@:80"));
	EXPECT_TRUE(ABNF::IsAuthority("username:pass:@:80"));
	EXPECT_TRUE(ABNF::IsAuthority("username:pass:xxx@:80"));
	EXPECT_TRUE(ABNF::IsAuthority("username::@:80"));

	EXPECT_FALSE(ABNF::IsAuthority("username@:aaa"));
	EXPECT_FALSE(ABNF::IsAuthority("username:pass:@:aaa"));
	EXPECT_FALSE(ABNF::IsAuthority("username:pass:xxx@:aaa"));
	EXPECT_FALSE(ABNF::IsAuthority("username::@:aaa"));
}

TEST(uri_abnf, is_authority_userinfo_host_port)
{
	EXPECT_TRUE(ABNF::IsAuthority("username@host:80"));
	EXPECT_TRUE(ABNF::IsAuthority("username:pass:@host:80"));
	EXPECT_TRUE(ABNF::IsAuthority("username:pass:xxx@host:80"));
	EXPECT_TRUE(ABNF::IsAuthority("username::@host:80"));

	EXPECT_FALSE(ABNF::IsAuthority("username@host:aaa"));
	EXPECT_FALSE(ABNF::IsAuthority("username:pass:@:aaa"));
	EXPECT_FALSE(ABNF::IsAuthority("username:pass:xxx@:aaa"));
	EXPECT_FALSE(ABNF::IsAuthority("username::@:aaa"));
	EXPECT_FALSE(ABNF::IsAuthority("username::@:aaa"));
}
