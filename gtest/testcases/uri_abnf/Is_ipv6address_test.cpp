#include "gtest.h"

#include "uri_abnf.hpp"

// IPv6address   =                           6( h16 ":" ) ls32
//              /                       "::" 5( h16 ":" ) ls32
//              / [               h16 ] "::" 4( h16 ":" ) ls32
//              / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
//              / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
//              / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
//              / [ *4( h16 ":" ) h16 ] "::"              ls32
//              / [ *5( h16 ":" ) h16 ] "::"              h16
//              / [ *6( h16 ":" ) h16 ] "::"

TEST(uri_abnf, is_ipv6address_no_dcolon_true)
{
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4:5:6:7:8"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4:5:6:0.0.0.0"));
}

TEST(uri_abnf, is_ipv6address_dcolon_true)
{
	EXPECT_TRUE(ABNF::IsIPv6address("::"));
	EXPECT_TRUE(ABNF::IsIPv6address("::1"));
	EXPECT_TRUE(ABNF::IsIPv6address("1::"));
	EXPECT_TRUE(ABNF::IsIPv6address("::1:2"));
	EXPECT_TRUE(ABNF::IsIPv6address("1::2"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2::"));
	EXPECT_TRUE(ABNF::IsIPv6address("::1:2:3"));
	EXPECT_TRUE(ABNF::IsIPv6address("1::2:3"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2::3"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3::"));
	EXPECT_TRUE(ABNF::IsIPv6address("::1:2:3:4"));
	EXPECT_TRUE(ABNF::IsIPv6address("1::2:3:4"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2::3:4"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3::4"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4::"));
	EXPECT_TRUE(ABNF::IsIPv6address("::1:2:3:4:5"));
	EXPECT_TRUE(ABNF::IsIPv6address("1::2:3:4:5"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2::3:4:5"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3::4:5"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4::5"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4:5::"));
	EXPECT_TRUE(ABNF::IsIPv6address("::1:2:3:4:5:6"));
	EXPECT_TRUE(ABNF::IsIPv6address("1::2:3:4:5:6"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2::3:4:5:6"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3::4:5:6"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4::5:6"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4:5::6"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4:5:6::"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4:5:6:7"));
	EXPECT_TRUE(ABNF::IsIPv6address("::1:2:3:4:5:6:7"));
	EXPECT_TRUE(ABNF::IsIPv6address("1::2:3:4:5:6:7"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2::3:4:5:6:7"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3::4:5:6:7"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4::5:6:7"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4:5::6:7"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4:5:6::7"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4:5:6:7::"));
}

TEST(uri_abnf, is_ipv6address_dcolon_ipv4_true)
{
	EXPECT_TRUE(ABNF::IsIPv6address("::0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("::1:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1::0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("::1:2:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1::2:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2::0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("::1:2:3:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1::2:3:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2::3:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3::0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("::1:2:3:4:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1::2:3:4:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2::3:4:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3::4:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4::0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("::1:2:3:4:5:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1::2:3:4:5:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2::3:4:5:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3::4:5:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4::5:0.0.0.0"));
	EXPECT_TRUE(ABNF::IsIPv6address("1:2:3:4:5::0.0.0.0"));
}

TEST(uri_abnf, is_ipv6address_no_dcolon_false)
{
	EXPECT_FALSE(ABNF::IsIPv6address(""));
	EXPECT_FALSE(ABNF::IsIPv6address(":"));
	EXPECT_FALSE(ABNF::IsIPv6address("1"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:6"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:6:7"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:6:7:8:9"));
	EXPECT_FALSE(ABNF::IsIPv6address(":1:2:3:4:5:6:7:8"));
	EXPECT_FALSE(ABNF::IsIPv6address(":1:2:3:4:5:6:7:8:"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:6:7:8:"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:6:7:"));
	EXPECT_FALSE(ABNF::IsIPv6address(":1:2:3:4:5:6:7:"));
	EXPECT_FALSE(ABNF::IsIPv6address(":1:2:3:4:5:6:7"));
	EXPECT_FALSE(ABNF::IsIPv6address(":1:2:3:4:5:6:7:"));
	EXPECT_FALSE(ABNF::IsIPv6address(":1:2:3:4:5:6:"));
}

TEST(uri_abnf, is_ipv6address_dcolon_false)
{
	EXPECT_FALSE(ABNF::IsIPv6address(":::"));
	EXPECT_FALSE(ABNF::IsIPv6address(":::1"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:::"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:::3"));
	EXPECT_FALSE(ABNF::IsIPv6address("::::"));
	EXPECT_FALSE(ABNF::IsIPv6address("::1::"));
	EXPECT_FALSE(ABNF::IsIPv6address("1::1::"));
	EXPECT_FALSE(ABNF::IsIPv6address("::1::1"));
	EXPECT_FALSE(ABNF::IsIPv6address("1::1::1"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4::::5:6:7"));
	EXPECT_FALSE(ABNF::IsIPv6address("::1:2:3:4::5:6:7"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4::5:6::7"));
	EXPECT_FALSE(ABNF::IsIPv6address("::1:2:3:4:5:6:7:8"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:6:7:8::"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4::5:6:7:8"));
	EXPECT_FALSE(ABNF::IsIPv6address("1::2:3:4:5:6:7:8"));
	EXPECT_FALSE(ABNF::IsIPv6address(":1::2:3:4:5:6:7"));
	EXPECT_FALSE(ABNF::IsIPv6address(":::12:3:4:5:6:7"));
	EXPECT_FALSE(ABNF::IsIPv6address("1::2:3:4:5:6:7:"));
	EXPECT_FALSE(ABNF::IsIPv6address(":1::2:3:4:5:6:7:"));
	EXPECT_FALSE(ABNF::IsIPv6address("1::2:3:4:5:6:"));
	EXPECT_FALSE(ABNF::IsIPv6address(":1::2:3:4:5:6:"));
}

TEST(uri_abnf, is_ipv6address_no_dcolon_ipv4_false)
{
	EXPECT_FALSE(ABNF::IsIPv6address("0.0.0.0:1:2:3:4:5:6"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:0.0.0.0:4:5:6"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:0.0.0.0"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:6:7:0.0.0.0"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:6:7:8:0.0.0.0"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:6:0.0.0.0:"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:6:7:0.0.0.0:"));
}

TEST(uri_abnf, is_ipv6address_dcolon_ipv4_false)
{
	EXPECT_FALSE(ABNF::IsIPv6address("0.0.0.0::1:2:3:4:5"));
	EXPECT_FALSE(ABNF::IsIPv6address("0.0.0.0:1::2:3:4:5"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:::0.0.0.0"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:0.0.0.0:4:5::"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:0.0.0.0::"));
	EXPECT_FALSE(ABNF::IsIPv6address("1:2:3:4:5:0.0.0.0:::"));
	EXPECT_FALSE(ABNF::IsIPv6address("1::2:3:4:5:6:0.0.0.0"));
	EXPECT_FALSE(ABNF::IsIPv6address("1::2:3:4:5:6:7:0.0.0.0"));
	EXPECT_FALSE(ABNF::IsIPv6address("1::2:3:4:5:0.0.0.0:"));
	EXPECT_FALSE(ABNF::IsIPv6address(":1::2:3:4:5:0.0.0.0:"));
	EXPECT_FALSE(ABNF::IsIPv6address("1::2:3:4:0.0.0.0:"));
	EXPECT_FALSE(ABNF::IsIPv6address(":1::2:3:4:0.0.0.0:"));
}
