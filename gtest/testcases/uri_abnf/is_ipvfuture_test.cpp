#include "gtest.h"

#include "parse_ip.hpp"

TEST(uri_abnf, is_ipvfuture_true)
{
	EXPECT_TRUE(ABNF::IsIPvFuture("v1.1"));
	EXPECT_TRUE(ABNF::IsIPvFuture("va.a"));
	EXPECT_TRUE(ABNF::IsIPvFuture("va.z"));
	EXPECT_TRUE(ABNF::IsIPvFuture("v1.1.1"));
	EXPECT_TRUE(ABNF::IsIPvFuture("v1.1.:!:$:&:':(:):*:+:,:;:=:-:.:_:~:"));
}

TEST(uri_abnf, is_ipvfuture_false)
{
	EXPECT_FALSE(ABNF::IsIPvFuture(""));
	EXPECT_FALSE(ABNF::IsIPvFuture("1"));
	EXPECT_FALSE(ABNF::IsIPvFuture("."));
	EXPECT_FALSE(ABNF::IsIPvFuture("1."));
	EXPECT_FALSE(ABNF::IsIPvFuture("v"));
	EXPECT_FALSE(ABNF::IsIPvFuture("v1"));
	EXPECT_FALSE(ABNF::IsIPvFuture("v1."));
	EXPECT_FALSE(ABNF::IsIPvFuture("v.1"));
	EXPECT_FALSE(ABNF::IsIPvFuture("vz.z"));
	EXPECT_FALSE(ABNF::IsIPvFuture("v:.z"));
	EXPECT_FALSE(ABNF::IsIPvFuture("v1.@"));
	EXPECT_FALSE(ABNF::IsIPvFuture("v1.?"));
	EXPECT_FALSE(ABNF::IsIPvFuture("v1.\""));
	EXPECT_FALSE(ABNF::IsIPvFuture("vv.v"));
	EXPECT_FALSE(ABNF::IsIPvFuture("vvvvvvvvvvv.v"));
}