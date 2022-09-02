// path-absolute = "/" [ segment-nz *( "/" segment ) ]
// segment       = *pchar
// segment-nz    = 1*pchar
#include "gtest.h"

#include "parse_path.hpp"

TEST(uri_abnf, is_path_rootless_true_case)
{
	EXPECT_TRUE(ABNF::IsPathRootless("aaa"));
	EXPECT_TRUE(ABNF::IsPathRootless("aaa/"));
	EXPECT_TRUE(ABNF::IsPathRootless("aaa/ZZZ"));
	EXPECT_TRUE(ABNF::IsPathRootless("aaa/ZZZ/"));
	EXPECT_TRUE(ABNF::IsPathRootless("aaa//ZZZ/"));
}

TEST(uri_abnf, is_path_rootless_false_case)
{
	EXPECT_FALSE(ABNF::IsPathRootless(""));
	EXPECT_FALSE(ABNF::IsPathRootless("/aaa"));
	EXPECT_FALSE(ABNF::IsPathRootless("/aaa/"));
	EXPECT_FALSE(ABNF::IsPathRootless("/aaa/ZZZ"));
	EXPECT_FALSE(ABNF::IsPathRootless("/aaa/ZZZ/"));
	EXPECT_FALSE(ABNF::IsPathRootless("/aaa/ZZZ/?bbb=%00"));
	EXPECT_FALSE(ABNF::IsPathRootless("/aaa/ZZZ/?bbb=%00"));
}
