// path-absolute = "/" [ segment-nz *( "/" segment ) ]
// segment       = *pchar
// segment-nz    = 1*pchar
#include "gtest.h"

#include "uri_abnf.hpp"

TEST(uri_abnf, is_path_absolute_true_case)
{
	ASSERT_TRUE("/");
	ASSERT_TRUE("/aaa");
	ASSERT_TRUE("/aaa/");
	ASSERT_TRUE("/aaa/ZZZ");
	ASSERT_TRUE("/aaa/ZZZ/");
	ASSERT_TRUE("/aaa/ZZZ/?bbb=%00");
}

TEST(uri_abnf, is_path_absolute_false_case)
{
	ASSERT_FALSE("");
	ASSERT_FALSE("aaa");
	ASSERT_FALSE("aaa/");
	ASSERT_FALSE("aaa/ZZZ");
	ASSERT_FALSE("aaa/ZZZ/");
	ASSERT_FALSE("aaa/ZZZ/?bbb=%00");
}
