// path-absolute = "/" [ segment-nz *( "/" segment ) ]
// segment       = *pchar
// segment-nz    = 1*pchar
#include "gtest.h"

#include "uri_abnf.hpp"

TEST(uri_abnf, is_path_absolute_true_case)
{
	EXPECT_TRUE(ABNF::IsPathAbsolute("/"));
	EXPECT_TRUE(ABNF::IsPathAbsolute("/aaa"));
	EXPECT_TRUE(ABNF::IsPathAbsolute("/aaa/"));
	EXPECT_TRUE(ABNF::IsPathAbsolute("/aaa/ZZZ"));
	EXPECT_TRUE(ABNF::IsPathAbsolute("/aaa/ZZZ/"));
}

TEST(uri_abnf, is_path_absolute_false_case)
{
	EXPECT_FALSE(ABNF::IsPathAbsolute(""));
	EXPECT_FALSE(ABNF::IsPathAbsolute("aaa"));
	EXPECT_FALSE(ABNF::IsPathAbsolute("aaa/"));
	EXPECT_FALSE(ABNF::IsPathAbsolute("aaa/ZZZ"));
	EXPECT_FALSE(ABNF::IsPathAbsolute("aaa/ZZZ/"));
	EXPECT_FALSE(ABNF::IsPathAbsolute("aaa/ZZZ/?bbb=%00"));
	EXPECT_FALSE(ABNF::IsPathAbsolute("/aaa/ZZZ/?bbb=%00"));
}
