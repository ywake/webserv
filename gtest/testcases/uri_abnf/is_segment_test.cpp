// segment       = *pchar
#include "gtest.h"

#include "parse_path_relative.hpp"

TEST(uri_abnf, is_segment_true_case_empty)
{
	ASSERT_TRUE(ABNF::IsSegment(""));
}

TEST(uri_abnf, is_segment_true_case_unreserved)
{
	ASSERT_TRUE(ABNF::IsSegment("abcdefg"));
	ASSERT_TRUE(ABNF::IsSegment("XYZ"));
	ASSERT_TRUE(ABNF::IsSegment("-._~"));
}

TEST(uri_abnf, is_segment_true_case_pct_encoded)
{
	ASSERT_TRUE(ABNF::IsSegment("%00%99%AA%FF"));
}

TEST(uri_abnf, is_segment_true_case_sub_delims)
{
	ASSERT_TRUE(ABNF::IsSegment("!$&'()*+,;="));
}

TEST(uri_abnf, is_segment_true_case_unique_char_set)
{
	ASSERT_TRUE(ABNF::IsSegment(":@"));
}

TEST(uri_abnf, is_segment_true_case_mix)
{
	ASSERT_TRUE(ABNF::IsSegment("abc-XYZ.%27:%41%61"));
}

TEST(uri_abnf, is_segment_false_case)
{
	ASSERT_FALSE(ABNF::IsSegment("abc-XYZ.%2:%41%61"));
	ASSERT_FALSE(ABNF::IsSegment("\\abc-XYZ.%27:%41%61"));
}
