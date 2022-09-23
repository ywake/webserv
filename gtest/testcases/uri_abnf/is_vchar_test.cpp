#include "gtest.h"

#include "parse_abnf_core_rules.hpp"

TEST(is_vchar, is_vchar)
{
	for (int c = 0x00; c <= 0xFF; c++) {
		if ((0x21 <= c && c <= 0x7E)) {
			EXPECT_TRUE(ABNF::IsVchar(c));
		} else {
			EXPECT_FALSE(ABNF::IsVchar(c));
		}
	}
}
