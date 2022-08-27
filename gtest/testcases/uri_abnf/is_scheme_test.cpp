#include "gtest.h"

#include "uri_abnf.hpp"

TEST(uri_abnf, is_scheme_true_test)
{
	EXPECT_TRUE(ABNF::IsScheme("http"));
	EXPECT_TRUE(ABNF::IsScheme("https"));
	EXPECT_TRUE(ABNF::IsScheme("FTP"));
	EXPECT_TRUE(ABNF::IsScheme("a1c-.+"));
}

TEST(uri_abnf, is_scheme_false_test)
{
	EXPECT_FALSE(ABNF::IsScheme(""));
	EXPECT_FALSE(ABNF::IsScheme("1abc"));
	EXPECT_FALSE(ABNF::IsScheme("+abc"));
	EXPECT_FALSE(ABNF::IsScheme("-abc"));
	EXPECT_FALSE(ABNF::IsScheme(".abc"));
	EXPECT_FALSE(ABNF::IsScheme("http/"));
	EXPECT_FALSE(ABNF::IsScheme("http?"));
	EXPECT_FALSE(ABNF::IsScheme("http|"));
}
