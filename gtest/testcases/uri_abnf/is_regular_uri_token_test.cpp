#include "gtest.h"
#include "parse_path_utils.hpp"
#include "parse_uri_utils.hpp"

// // IsRegularUriToken
// // IsRegularUriTokenOnly

static const std::string kNgSet = "\"/:<>?@[\\]{}"; //適当 漏れありそう

TEST(uri_abnf, is_regular_uri_token_true)
{
	EXPECT_TRUE(ABNF::IsRegularUriToken(std::string("\0", 1), std::string("\0", 1)));
	EXPECT_TRUE(ABNF::IsRegularUriToken("\"", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriToken("/", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriToken(":", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriToken("<", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriToken("=", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriToken(">", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriToken("?", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriToken("@", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriToken("[", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriToken("\\", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriToken("]", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriToken("{", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriToken("}", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriToken("a", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("b", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("c", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("X", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("Y", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("Z", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("1", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("2", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("3", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("4", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("5", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("6", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("7", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("8", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("9", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("0", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("-", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken(".", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("_", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("~", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("!", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("$", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("&", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("'", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("(", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken(")", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("*", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("+", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken(",", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken(";", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("=", ""));
}

TEST(uri_abnf, is_regular_uri_token_pct)
{
	EXPECT_TRUE(ABNF::IsRegularUriToken("%00", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("%01", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("%0F", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("%0f", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("%ff", ""));
	EXPECT_TRUE(ABNF::IsRegularUriToken("%FF", ""));

	EXPECT_FALSE(ABNF::IsRegularUriToken("%", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("%a", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("%0", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("%%%", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("%0z", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("%0g", ""));
}

TEST(uri_abnf, is_regular_uri_token_false)
{
	EXPECT_FALSE(ABNF::IsRegularUriToken("", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("\"", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("/", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken(":", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("<", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken(">", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("?", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("@", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("[", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("\\", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("]", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("{", ""));
	EXPECT_FALSE(ABNF::IsRegularUriToken("}", ""));
}

TEST(uri_abnf, is_regular_uri_only_token_true)
{
	EXPECT_TRUE(
		ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar(std::string("\0", 1)), std::string("\0", 1))
	);
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("\""), kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("/"), kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar(":"), kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("<"), kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("="), kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar(">"), kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("?"), kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("@"), kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("["), kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("\\"), kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("]"), kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("{"), kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("}"), kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("abcdefg"), ""));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("0123456789"), ""));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("%aa0123456789"), ""));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("%ff0123456789"), ""));
	EXPECT_TRUE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("%010123456789"), ""));
}

TEST(uri_abnf, is_regular_uri_only_token_false)
{
	EXPECT_FALSE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar(std::string("\0", 1)), ""));
	EXPECT_FALSE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar(std::string("\0", 1)), kNgSet));
	EXPECT_FALSE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar(kNgSet), ""));
	EXPECT_FALSE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("a" + kNgSet), ""));
	EXPECT_FALSE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar(kNgSet + "a"), ""));
	EXPECT_FALSE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("a" + kNgSet + "a"), ""));
	EXPECT_FALSE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("%0z0123456789"), ""));
	EXPECT_FALSE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("abcdefg%"), ""));
	EXPECT_FALSE(ABNF::IsRegularUriTokenOnly(ABNF::TokenizePchar("abcdef%g"), ""));
}
