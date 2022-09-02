

// query = *( pchar / "/" / "?" )
#include "gtest.h"

#include "parse_uri.hpp"

TEST(uri_abnf, is_query_true_test_pchar)
{
	ASSERT_TRUE(ABNF::IsQuery("abcdefg"));
	ASSERT_TRUE(ABNF::IsQuery("XYZ"));
	ASSERT_TRUE(ABNF::IsQuery("123890"));
	ASSERT_TRUE(ABNF::IsQuery("%00%99%AA%FF"));
	ASSERT_TRUE(ABNF::IsQuery(":@"));
	ASSERT_TRUE(ABNF::IsQuery("abc-XYZ.%27:%41%61"));
}

TEST(uri_abnf, is_query_true_test_unique_char_set)
{
	ASSERT_TRUE(ABNF::IsQuery("/?"));
}

TEST(uri_abnf, is_query_true_test_mix)
{
	ASSERT_TRUE(ABNF::IsQuery("abc/XYZ/%41?@=123&a=890"));
}

TEST(uri_abnf, is_query_false_test)
{
	ASSERT_FALSE(ABNF::IsQuery("abc/XYZ\\%41?@=123&a=890"));
	ASSERT_FALSE(ABNF::IsQuery("abc/XYZ/%41?@=123#890"));
}
