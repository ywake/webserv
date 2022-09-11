#include "gtest.h"

#include "parse_uri_relative.hpp"

TEST(uri_abnf, IsUnreserved_alpha)
{
	ASSERT_TRUE(ABNF::IsUnreserved('a'));
	ASSERT_TRUE(ABNF::IsUnreserved('b'));
	ASSERT_TRUE(ABNF::IsUnreserved('c'));
	ASSERT_TRUE(ABNF::IsUnreserved('X'));
	ASSERT_TRUE(ABNF::IsUnreserved('Y'));
	ASSERT_TRUE(ABNF::IsUnreserved('Z'));
}

TEST(uri_abnf, IsUnreserved_DIGIT)
{
	ASSERT_TRUE(ABNF::IsUnreserved('1'));
	ASSERT_TRUE(ABNF::IsUnreserved('2'));
	ASSERT_TRUE(ABNF::IsUnreserved('3'));
	ASSERT_TRUE(ABNF::IsUnreserved('4'));
	ASSERT_TRUE(ABNF::IsUnreserved('5'));
	ASSERT_TRUE(ABNF::IsUnreserved('6'));
	ASSERT_TRUE(ABNF::IsUnreserved('7'));
	ASSERT_TRUE(ABNF::IsUnreserved('8'));
	ASSERT_TRUE(ABNF::IsUnreserved('9'));
	ASSERT_TRUE(ABNF::IsUnreserved('0'));
}

TEST(uri_abnf, IsUnreserved_hyphen)
{
	ASSERT_TRUE(ABNF::IsUnreserved('-'));
}

TEST(uri_abnf, IsUnreserved_dot)
{
	ASSERT_TRUE(ABNF::IsUnreserved('.'));
}

TEST(uri_abnf, IsUnreserved_under_score)
{
	ASSERT_TRUE(ABNF::IsUnreserved('_'));
}

TEST(uri_abnf, IsUnreserved_tilda)
{
	ASSERT_TRUE(ABNF::IsUnreserved('~'));
}

TEST(uri_abnf, IsUnreserved_false_sign)
{
	ASSERT_FALSE(ABNF::IsUnreserved(' '));
	ASSERT_FALSE(ABNF::IsUnreserved('!'));
	ASSERT_FALSE(ABNF::IsUnreserved('"'));
	ASSERT_FALSE(ABNF::IsUnreserved('#'));
	ASSERT_FALSE(ABNF::IsUnreserved('$'));
	ASSERT_FALSE(ABNF::IsUnreserved('%'));
	ASSERT_FALSE(ABNF::IsUnreserved('&'));
	ASSERT_FALSE(ABNF::IsUnreserved('\''));
	ASSERT_FALSE(ABNF::IsUnreserved('('));
	ASSERT_FALSE(ABNF::IsUnreserved(')'));
	ASSERT_FALSE(ABNF::IsUnreserved('*'));
	ASSERT_FALSE(ABNF::IsUnreserved('+'));
	ASSERT_FALSE(ABNF::IsUnreserved(','));
	ASSERT_FALSE(ABNF::IsUnreserved('/'));
	ASSERT_FALSE(ABNF::IsUnreserved(':'));
	ASSERT_FALSE(ABNF::IsUnreserved(';'));
	ASSERT_FALSE(ABNF::IsUnreserved('<'));
	ASSERT_FALSE(ABNF::IsUnreserved('='));
	ASSERT_FALSE(ABNF::IsUnreserved('>'));
	ASSERT_FALSE(ABNF::IsUnreserved('?'));
	ASSERT_FALSE(ABNF::IsUnreserved('@'));
	ASSERT_FALSE(ABNF::IsUnreserved('['));
	ASSERT_FALSE(ABNF::IsUnreserved('\\'));
	ASSERT_FALSE(ABNF::IsUnreserved(']'));
	ASSERT_FALSE(ABNF::IsUnreserved('['));
	ASSERT_FALSE(ABNF::IsUnreserved('`'));
	ASSERT_FALSE(ABNF::IsUnreserved('{'));
	ASSERT_FALSE(ABNF::IsUnreserved('|'));
	ASSERT_FALSE(ABNF::IsUnreserved('}'));
	ASSERT_FALSE(ABNF::IsUnreserved('\0'));
}
