#include <cstdlib>

#include "gtest.h"
#include "uri_abnf.hpp"

std::size_t wrap_get_len_until(const std::string src, const std::string delim, std::size_t start)
{
	return ABNF::get_len_until(src, delim, start);
}

TEST(uri_abnf, get_len_until)
{
	EXPECT_EQ(wrap_get_len_until("", "", 0), 0);
	EXPECT_EQ(wrap_get_len_until("", "", 1), 0);
	EXPECT_EQ(wrap_get_len_until("0", "", 0), 0);
	EXPECT_EQ(wrap_get_len_until("0", "", 1), 0);
	EXPECT_EQ(wrap_get_len_until("0|", "|", 0), 1);
	EXPECT_EQ(wrap_get_len_until("0|", "|", 1), 0);
	EXPECT_EQ(wrap_get_len_until("0|", "|", 10), 0);
	EXPECT_EQ(wrap_get_len_until("0||", "|", 0), 1);
	EXPECT_EQ(wrap_get_len_until("0||", "|", 1), 0);
	EXPECT_EQ(wrap_get_len_until("0|2|", "|", 0), 1);
	EXPECT_EQ(wrap_get_len_until("0|2|", "|", 1), 0);
	EXPECT_EQ(wrap_get_len_until("|", "|", 0), 0);
	EXPECT_EQ(wrap_get_len_until("|", "|", 1), 0);
	EXPECT_EQ(wrap_get_len_until("||2", "|", 2), 1);
	EXPECT_EQ(wrap_get_len_until("||23", "|", 2), 2);
	EXPECT_EQ(wrap_get_len_until("||2|", "|", 2), 1);
	EXPECT_EQ(wrap_get_len_until("||2|4", "|", 2), 1);
	EXPECT_EQ(wrap_get_len_until("||23|5", "|", 2), 2);
	EXPECT_EQ(wrap_get_len_until("0||", "||", 0), 1);
	EXPECT_EQ(wrap_get_len_until("||2", "||", 0), 0);
	EXPECT_EQ(wrap_get_len_until("||2", "||", 1), 2);
	EXPECT_EQ(wrap_get_len_until("||2", "||", 2), 1);
}
