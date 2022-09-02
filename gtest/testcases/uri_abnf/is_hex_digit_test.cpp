#include "gtest.h"

#include "parse_abnf_core_rules.hpp"

// "%" HEXDIG HEXDIG
TEST(uri_abnf, IsHexDigit_true)
{
	ASSERT_TRUE(ABNF::IsHexDigit('0'));
	ASSERT_TRUE(ABNF::IsHexDigit('1'));
	ASSERT_TRUE(ABNF::IsHexDigit('2'));
	ASSERT_TRUE(ABNF::IsHexDigit('3'));
	ASSERT_TRUE(ABNF::IsHexDigit('4'));
	ASSERT_TRUE(ABNF::IsHexDigit('5'));
	ASSERT_TRUE(ABNF::IsHexDigit('6'));
	ASSERT_TRUE(ABNF::IsHexDigit('7'));
	ASSERT_TRUE(ABNF::IsHexDigit('8'));
	ASSERT_TRUE(ABNF::IsHexDigit('9'));
	ASSERT_TRUE(ABNF::IsHexDigit('A'));
	ASSERT_TRUE(ABNF::IsHexDigit('B'));
	ASSERT_TRUE(ABNF::IsHexDigit('C'));
	ASSERT_TRUE(ABNF::IsHexDigit('D'));
	ASSERT_TRUE(ABNF::IsHexDigit('E'));
	ASSERT_TRUE(ABNF::IsHexDigit('F'));
	ASSERT_TRUE(ABNF::IsHexDigit('a'));
	ASSERT_TRUE(ABNF::IsHexDigit('b'));
	ASSERT_TRUE(ABNF::IsHexDigit('c'));
	ASSERT_TRUE(ABNF::IsHexDigit('d'));
	ASSERT_TRUE(ABNF::IsHexDigit('e'));
	ASSERT_TRUE(ABNF::IsHexDigit('f'));
}

TEST(uri_abnf, IsHexDigit_false)
{
	ASSERT_FALSE(ABNF::IsHexDigit(' '));
	ASSERT_FALSE(ABNF::IsHexDigit('/'));
	ASSERT_FALSE(ABNF::IsHexDigit(':'));
	ASSERT_FALSE(ABNF::IsHexDigit('@'));
	ASSERT_FALSE(ABNF::IsHexDigit('G'));
	ASSERT_FALSE(ABNF::IsHexDigit('['));
	ASSERT_FALSE(ABNF::IsHexDigit('z'));
	ASSERT_FALSE(ABNF::IsHexDigit('}'));
}
