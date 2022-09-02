#include "gtest.h"

#include "parse_uri_relative.hpp"

// "%" HEXDIG HEXDIG
TEST(uri_abnf, IsSubDelims_true)
{
	ASSERT_TRUE(ABNF::IsSubDelims('!'));
	ASSERT_TRUE(ABNF::IsSubDelims('$'));
	ASSERT_TRUE(ABNF::IsSubDelims('&'));
	ASSERT_TRUE(ABNF::IsSubDelims('\''));
	ASSERT_TRUE(ABNF::IsSubDelims('('));
	ASSERT_TRUE(ABNF::IsSubDelims(')'));
	ASSERT_TRUE(ABNF::IsSubDelims('*'));
	ASSERT_TRUE(ABNF::IsSubDelims('+'));
	ASSERT_TRUE(ABNF::IsSubDelims(','));
	ASSERT_TRUE(ABNF::IsSubDelims(';'));
	ASSERT_TRUE(ABNF::IsSubDelims('='));
}

TEST(uri_abnf, IsSubDelims_false)
{
	ASSERT_FALSE(ABNF::IsSubDelims('\"'));
	ASSERT_FALSE(ABNF::IsSubDelims('#'));
	ASSERT_FALSE(ABNF::IsSubDelims('%'));
	ASSERT_FALSE(ABNF::IsSubDelims('&'));
	ASSERT_FALSE(ABNF::IsSubDelims('-'));
	ASSERT_FALSE(ABNF::IsSubDelims('.'));
	ASSERT_FALSE(ABNF::IsSubDelims('/'));
	ASSERT_FALSE(ABNF::IsSubDelims('0'));
	ASSERT_FALSE(ABNF::IsSubDelims('9'));
	ASSERT_FALSE(ABNF::IsSubDelims(':'));
	ASSERT_FALSE(ABNF::IsSubDelims('<'));
	ASSERT_FALSE(ABNF::IsSubDelims('?'));
	ASSERT_FALSE(ABNF::IsSubDelims('@'));
	ASSERT_FALSE(ABNF::IsSubDelims('A'));
	ASSERT_FALSE(ABNF::IsSubDelims('Z'));
	ASSERT_FALSE(ABNF::IsSubDelims('['));
	ASSERT_FALSE(ABNF::IsSubDelims('\\'));
	ASSERT_FALSE(ABNF::IsSubDelims('_'));
	ASSERT_FALSE(ABNF::IsSubDelims('`'));
	ASSERT_FALSE(ABNF::IsSubDelims('a'));
	ASSERT_FALSE(ABNF::IsSubDelims('z'));
	ASSERT_FALSE(ABNF::IsSubDelims('{'));
	ASSERT_FALSE(ABNF::IsSubDelims('|'));
	ASSERT_FALSE(ABNF::IsSubDelims('}'));
	ASSERT_FALSE(ABNF::IsSubDelims('~'));
}
