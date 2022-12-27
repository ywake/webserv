#include "gtest.h"

#include "percent_encode.hpp"

TEST(percent_decode, ok)
{
	EXPECT_EQ(uri::PercentDecode("").Val(), "");
	EXPECT_EQ(uri::PercentDecode("%20").Val(), " ");
	EXPECT_EQ(uri::PercentDecode("%0a").Val(), "\n");
	EXPECT_EQ(uri::PercentDecode("%E3%81%82").Val(), "あ");
	EXPECT_EQ(uri::PercentDecode("a%E3%81%82n").Val(), "aあn");
}

TEST(percent_decode, ng)
{
	EXPECT_TRUE(uri::PercentDecode("%00").IsErr());
	EXPECT_TRUE(uri::PercentDecode("%ag").IsErr());
	EXPECT_TRUE(uri::PercentDecode("%0g").IsErr());
	EXPECT_TRUE(uri::PercentDecode("a%00x").IsErr());
	EXPECT_TRUE(uri::PercentDecode("%0x").IsErr());
}
