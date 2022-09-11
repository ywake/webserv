#include "gtest.h"

#include "parse_ip.hpp"

TEST(uri_abnf, is_dec_octet_test)
{
	EXPECT_TRUE(ABNF::IsDecOctet("0"));
	EXPECT_TRUE(ABNF::IsDecOctet("1"));
	EXPECT_TRUE(ABNF::IsDecOctet("20"));
	EXPECT_TRUE(ABNF::IsDecOctet("199"));
	EXPECT_TRUE(ABNF::IsDecOctet("230"));
	EXPECT_TRUE(ABNF::IsDecOctet("254"));
	EXPECT_TRUE(ABNF::IsDecOctet("255"));

	EXPECT_FALSE(ABNF::IsDecOctet(""));
	EXPECT_FALSE(ABNF::IsDecOctet("-"));
	EXPECT_FALSE(ABNF::IsDecOctet("+"));
	EXPECT_FALSE(ABNF::IsDecOctet(" "));
	EXPECT_FALSE(ABNF::IsDecOctet("-0"));
	EXPECT_FALSE(ABNF::IsDecOctet("-1"));
	EXPECT_FALSE(ABNF::IsDecOctet("+1"));
	EXPECT_FALSE(ABNF::IsDecOctet("256"));
	EXPECT_FALSE(ABNF::IsDecOctet("000"));
	EXPECT_FALSE(ABNF::IsDecOctet("0199"));
	EXPECT_FALSE(ABNF::IsDecOctet(" 1"));
	EXPECT_FALSE(ABNF::IsDecOctet("1 "));
	EXPECT_FALSE(ABNF::IsDecOctet("1 2"));
}
