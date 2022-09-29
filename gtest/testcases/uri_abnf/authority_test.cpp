#include "gtest.h"

#include "authority.hpp"
#include "http_exceptions.hpp"
#include "parse_authority.hpp"

// authority = [ userinfo "@" ] host [ ":" port ]
TEST(uri_abnf, authority_userinfo)
{
	EXPECT_EQ(Authority("@"), Authority());
	EXPECT_EQ(Authority("username@"), Authority("username", "", ""));
	EXPECT_EQ(Authority("username:pass@"), Authority("username:pass", "", ""));
	EXPECT_EQ(Authority("username:pass:@"), Authority("username:pass:", "", ""));
	EXPECT_EQ(Authority("username::@"), Authority("username::", "", ""));
	EXPECT_EQ(Authority("username:pass:xxx@"), Authority("username:pass:xxx", "", ""));

	EXPECT_THROW(Authority("username:pass:xxx"), http::BadRequestException);
}

TEST(uri_abnf, authority_host)
{
	EXPECT_EQ(Authority("example.com"), Authority("", "example.com", ""));
	EXPECT_EQ(Authority("example@com"), Authority("example", "com", ""));
	EXPECT_EQ(Authority(""), Authority(""));
}

TEST(uri_abnf, authority_port)
{
	EXPECT_EQ(Authority(":"), Authority("", "", ""));
	EXPECT_EQ(Authority(":80"), Authority("", "", "80"));

	EXPECT_THROW(Authority(":aaa"), http::BadRequestException);
}

TEST(uri_abnf, authority_userinfo_host)
{
	EXPECT_EQ(Authority("username@host"), Authority("username", "host", ""));
	EXPECT_EQ(Authority("username:pass@host"), Authority("username:pass", "host", ""));
	EXPECT_EQ(Authority("username:pass:@host"), Authority("username:pass:", "host", ""));
	EXPECT_EQ(Authority("username:pass:xxx@host"), Authority("username:pass:xxx", "host", ""));
	EXPECT_EQ(Authority("username::@host"), Authority("username::", "host", ""));
}

TEST(uri_abnf, authority_host_port)
{
	EXPECT_EQ(Authority("host:80"), Authority("", "host", "80"));
	EXPECT_THROW(Authority("host:aaa"), http::BadRequestException);
}

TEST(uri_abnf, authority_userinfo_port)
{
	EXPECT_EQ(Authority("username@:80"), Authority("username", "", "80"));
	EXPECT_EQ(Authority("username:pass:@:80"), Authority("username:pass:", "", "80"));
	EXPECT_EQ(Authority("username:pass:xxx@:80"), Authority("username:pass:xxx", "", "80"));
	EXPECT_EQ(Authority("username::@:80"), Authority("username::", "", "80"));

	EXPECT_THROW(Authority("username@:aaa"), http::BadRequestException);
	EXPECT_THROW(Authority("username:pass:@:aaa"), http::BadRequestException);
	EXPECT_THROW(Authority("username:pass:xxx@:aaa"), http::BadRequestException);
	EXPECT_THROW(Authority("username::@:aaa"), http::BadRequestException);
}

TEST(uri_abnf, authority_userinfo_host_port)
{
	EXPECT_EQ(Authority("username@host:80"), Authority("username", "host", "80"));
	EXPECT_EQ(Authority("username:pass:@host:80"), Authority("username:pass:", "host", "80"));
	EXPECT_EQ(Authority("username:pass:xxx@host:80"), Authority("username:pass:xxx", "host", "80"));
	EXPECT_EQ(Authority("username::@host:80"), Authority("username::", "host", "80"));

	EXPECT_THROW(Authority("username@host:aaa"), http::BadRequestException);
	EXPECT_THROW(Authority("username:pass:@:aaa"), http::BadRequestException);
	EXPECT_THROW(Authority("username:pass:xxx@:aaa"), http::BadRequestException);
	EXPECT_THROW(Authority("username::@:aaa"), http::BadRequestException);
	EXPECT_THROW(Authority("username::@:aaa"), http::BadRequestException);
}
