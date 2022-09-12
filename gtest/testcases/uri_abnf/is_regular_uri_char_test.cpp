#include "gtest.h"
#include "parse_uri_utils.hpp"

static const std::string kNgSet = "\"/:<>?@[\\]{}"; //適当 漏れありそう

TEST(uri_abnf, is_regular_uri_true)
{
	EXPECT_TRUE(ABNF::IsRegularUriChar('\0', std::string("\0", 1)));
	EXPECT_TRUE(ABNF::IsRegularUriChar('"', kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriChar('/', kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriChar(':', kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriChar('<', kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriChar('=', kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriChar('>', kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriChar('?', kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriChar('@', kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriChar('[', kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriChar('\\', kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriChar(']', kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriChar('{', kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriChar('}', kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriChar('a', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('b', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('c', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('X', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('Y', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('Z', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('1', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('2', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('3', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('4', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('5', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('6', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('7', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('8', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('9', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('0', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('-', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('.', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('_', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('~', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('!', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('$', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('&', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('\'', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('(', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar(')', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('*', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('+', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar(',', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar(';', ""));
	EXPECT_TRUE(ABNF::IsRegularUriChar('=', ""));
}

TEST(uri_abnf, is_regular_uri_false)
{
	EXPECT_FALSE(ABNF::IsRegularUriChar('\0', ""));
	EXPECT_FALSE(ABNF::IsRegularUriChar('"', ""));
	EXPECT_FALSE(ABNF::IsRegularUriChar('/', ""));
	EXPECT_FALSE(ABNF::IsRegularUriChar(':', ""));
	EXPECT_FALSE(ABNF::IsRegularUriChar('<', ""));
	EXPECT_FALSE(ABNF::IsRegularUriChar('>', ""));
	EXPECT_FALSE(ABNF::IsRegularUriChar('?', ""));
	EXPECT_FALSE(ABNF::IsRegularUriChar('@', ""));
	EXPECT_FALSE(ABNF::IsRegularUriChar('[', ""));
	EXPECT_FALSE(ABNF::IsRegularUriChar('\\', ""));
	EXPECT_FALSE(ABNF::IsRegularUriChar(']', ""));
	EXPECT_FALSE(ABNF::IsRegularUriChar('{', ""));
	EXPECT_FALSE(ABNF::IsRegularUriChar('}', ""));
}

TEST(uri_abnf, is_regular_uri_only_true)
{
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly(std::string("\0", 1), std::string("\0", 1)));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly("\"", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly("/", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly(":", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly("<", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly("=", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly(">", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly("?", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly("@", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly("[", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly("\\", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly("]", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly("{", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly("}", kNgSet));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly("abcdefg", ""));
	EXPECT_TRUE(ABNF::IsRegularUriCharOnly("0123456789", ""));
}

TEST(uri_abnf, is_regular_uri_only_false)
{
	EXPECT_FALSE(ABNF::IsRegularUriCharOnly("", std::string("\0", 1)));
	EXPECT_FALSE(ABNF::IsRegularUriCharOnly(ThinString(), std::string("\0", 1)));
	EXPECT_FALSE(ABNF::IsRegularUriCharOnly(std::string("\0", 1), ""));
	EXPECT_FALSE(ABNF::IsRegularUriCharOnly(std::string("\0", 1), kNgSet));
	EXPECT_FALSE(ABNF::IsRegularUriCharOnly(kNgSet, ""));
	EXPECT_FALSE(ABNF::IsRegularUriCharOnly("a" + kNgSet, ""));
	EXPECT_FALSE(ABNF::IsRegularUriCharOnly(kNgSet + "a", ""));
	EXPECT_FALSE(ABNF::IsRegularUriCharOnly("a" + kNgSet + "a", ""));
}
