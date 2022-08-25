// pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
#include "gtest.h"

#include "uri_abnf.hpp"

TEST(uri_abnf, is_pchar_unreserved)
{
	ASSERT_TRUE(ABNF::IsPchar("a"));
	ASSERT_TRUE(ABNF::IsPchar("z"));
	ASSERT_TRUE(ABNF::IsPchar("A"));
	ASSERT_TRUE(ABNF::IsPchar("Z"));
	ASSERT_TRUE(ABNF::IsPchar("0"));
	ASSERT_TRUE(ABNF::IsPchar("9"));
	ASSERT_TRUE(ABNF::IsPchar("-"));
	ASSERT_TRUE(ABNF::IsPchar("."));
	ASSERT_TRUE(ABNF::IsPchar("_"));
	ASSERT_TRUE(ABNF::IsPchar("~"));
}

TEST(uri_abnf, is_pchar_pct_encoded)
{
	// true case
	ASSERT_TRUE(ABNF::IsPchar("%00"));
	ASSERT_TRUE(ABNF::IsPchar("%99"));
	ASSERT_TRUE(ABNF::IsPchar("%AA"));
	ASSERT_TRUE(ABNF::IsPchar("%FF"));
	ASSERT_TRUE(ABNF::IsPchar("%aa"));
	ASSERT_TRUE(ABNF::IsPchar("%ff"));

	// false case
	ASSERT_FALSE(ABNF::IsPchar("%gg"));
	ASSERT_FALSE(ABNF::IsPchar("%GG"));
	ASSERT_FALSE(ABNF::IsPchar("%zz"));
	ASSERT_FALSE(ABNF::IsPchar("%ZZ"));
	ASSERT_FALSE(ABNF::IsPchar("%-A"));
	ASSERT_FALSE(ABNF::IsPchar("%A_"));
}

TEST(uri_abnf, is_pchar_sub_delims)
{
	ASSERT_TRUE(ABNF::IsPchar("!"));
	ASSERT_TRUE(ABNF::IsPchar("$"));
	ASSERT_TRUE(ABNF::IsPchar("&"));
	ASSERT_TRUE(ABNF::IsPchar("'"));
	ASSERT_TRUE(ABNF::IsPchar("("));
	ASSERT_TRUE(ABNF::IsPchar(")"));
	ASSERT_TRUE(ABNF::IsPchar("*"));
	ASSERT_TRUE(ABNF::IsPchar("+"));
	ASSERT_TRUE(ABNF::IsPchar(","));
	ASSERT_TRUE(ABNF::IsPchar(";"));
	ASSERT_TRUE(ABNF::IsPchar("="));
}

TEST(uri_abnf, is_pchar_unique_char_set)
{
	ASSERT_TRUE(ABNF::IsPchar(":"));
	ASSERT_TRUE(ABNF::IsPchar("@"));
}

TEST(uri_abnf, is_pchar_false_case)
{
	ASSERT_FALSE(ABNF::IsPchar(" "));
	ASSERT_FALSE(ABNF::IsPchar("\""));
	ASSERT_FALSE(ABNF::IsPchar("#"));
	ASSERT_FALSE(ABNF::IsPchar("%"));
	ASSERT_FALSE(ABNF::IsPchar("/"));
	ASSERT_FALSE(ABNF::IsPchar("<"));
	ASSERT_FALSE(ABNF::IsPchar(">"));
	ASSERT_FALSE(ABNF::IsPchar("["));
	ASSERT_FALSE(ABNF::IsPchar("]"));
	ASSERT_FALSE(ABNF::IsPchar("^"));
	ASSERT_FALSE(ABNF::IsPchar("`"));
	ASSERT_FALSE(ABNF::IsPchar("{"));
	ASSERT_FALSE(ABNF::IsPchar("|"));
	ASSERT_FALSE(ABNF::IsPchar("}"));
	ASSERT_FALSE(ABNF::IsPchar("\\"));
}
