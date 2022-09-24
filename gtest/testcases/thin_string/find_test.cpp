#include "gtest.h"

#include "thin_string.hpp"

#include <iostream>

TEST(thin_string, init)
{
	ThinString str("");
	EXPECT_EQ(str.len(), 0);
	EXPECT_EQ(str.size(), 0);
	EXPECT_EQ(str, "");
}

TEST(thin_string, len)
{
	ThinString str;
	EXPECT_EQ(str.len(), 0);
	str = ThinString("0123456789ABCDEF", 0, 16);
	EXPECT_EQ(str.len(), 16);
	str = ThinString("0123456789ABCDEF", 3, 10);
	EXPECT_EQ(str.len(), 10);
	str = ThinString("0123456789ABCDEF", 10, 10);
	EXPECT_EQ(str.len(), 6);
	str = ThinString("0123456789ABCDEF", 16, 10);
	EXPECT_EQ(str.len(), 0);
	str = ThinString("0123456789ABCDEF", 20, 10);
	EXPECT_EQ(str.len(), 0);
}

TEST(thin_string, size)
{
	ThinString str;
	EXPECT_EQ(str.size(), 0);
	str = ThinString("0123456789ABCDEF", 0, 16);
	EXPECT_EQ(str.size(), 16);
	str = ThinString("0123456789ABCDEF", 3, 10);
	EXPECT_EQ(str.size(), 10);
	str = ThinString("0123456789ABCDEF", 10, 10);
	EXPECT_EQ(str.size(), 6);
	str = ThinString("0123456789ABCDEF", 16, 10);
	EXPECT_EQ(str.size(), 0);
	str = ThinString("0123456789ABCDEF", 20, 10);
	EXPECT_EQ(str.size(), 0);
}

TEST(thin_string, ToString)
{
	ThinString str;
	EXPECT_EQ(str, "");
	str = ThinString("0123ABC", 0, 20);
	EXPECT_EQ(str, "0123ABC");
	str = ThinString("0123ABC", 0, 4);
	EXPECT_EQ(str, "0123");
	str = ThinString("ABC0123ABC", 3, 4);
	EXPECT_EQ(str, "0123");
	str = ThinString("ABC0123ABC", 7, 4);
	EXPECT_EQ(str, "ABC");
}

TEST(thin_string, substr)
{
	ThinString str("0123456789", 3, 10);
	ThinString act;
	act = str.substr(0, 5);
	EXPECT_EQ(act, "34567");
	act = str.substr(5, 5);
	EXPECT_EQ(act, "89");
	act = str.substr(10, 5);
	EXPECT_EQ(act, "");
}

TEST(thin_string, find)
{
	ThinString  str;
	std::string std;

	str = ThinString("0123456789ABCDEF", 3, 10);
	EXPECT_EQ(str.find("345"), 0);
	str = ThinString("aaa/bbb", 3, 10);
	EXPECT_EQ(str.find("/"), 0);
	std = "a";
	str = std;
	EXPECT_EQ(str.find('\0'), std.find('\0'));
	std += '\0';
	str = std;
	EXPECT_EQ(str.find('\0'), std.find('\0'));
}

TEST(thin_string, operator_equal)
{
	EXPECT_TRUE(ThinString("aaa") == ThinString("aaa"));
	EXPECT_TRUE(ThinString("aaa") == "aaa");
	EXPECT_TRUE(ThinString("aaa") == std::string("aaa"));

	EXPECT_TRUE(ThinString("abcdef", 0, 3) == ThinString("abc"));
	EXPECT_TRUE(ThinString("abcdef", 3, 3) == "def");
	EXPECT_TRUE(ThinString("abcdef", 3, 10) == std::string("def"));

	EXPECT_FALSE(ThinString("aaa") == ThinString(""));
	EXPECT_FALSE(ThinString("aaa") == "");
	EXPECT_FALSE(ThinString("aaa") == std::string(""));

	EXPECT_FALSE(ThinString("") == ThinString("bbb"));
	EXPECT_FALSE(ThinString("") == "bbb");
	EXPECT_FALSE(ThinString("") == std::string("bbb"));

	EXPECT_FALSE(ThinString("bb") == ThinString("bbb"));
	EXPECT_FALSE(ThinString("bb") == "bbb");
	EXPECT_FALSE(ThinString("bb") == std::string("bbb"));

	EXPECT_FALSE(ThinString("aaa") == ThinString("bbb"));
	EXPECT_FALSE(ThinString("aaa") == "bbb");
	EXPECT_FALSE(ThinString("aaa") == std::string("bbb"));

	EXPECT_FALSE(ThinString("abcdef") == ThinString("abcdef", 0, 4));
	EXPECT_FALSE(ThinString("abcdef") == "abcd");
	EXPECT_FALSE(ThinString("abcdef") == std::string("abcd"));
}

void test_divide_by(
	const std::string    &str,
	const std::string    &delim,
	const std::string    &first,
	const std::string    &second,
	ThinString::DelimFlag option = ThinString::DelimFlag::kKeepDelimLeft
)
{
	ThinString              thin(str);
	ThinString::ThinStrPair p = thin.DivideBy(delim);
	EXPECT_EQ(p.first, first);
	EXPECT_EQ(p.second, second);
}

TEST(thin_string, DivideBy)
{
	test_divide_by("aaa/bbb", "/", "aaa", "bbb");
	test_divide_by("a?b", "?", "a", "b");
	test_divide_by("ab?", "?", "ab", "");
	test_divide_by("?ab", "?", "", "ab");
	test_divide_by("ab", "?", "ab", "");
	test_divide_by("ab", "", "ab", "");
	test_divide_by("ab", "?/", "ab", "");
	test_divide_by("a?b", "??", "a?b", "");
	test_divide_by("a?b?c", "?", "a", "b?c");
	test_divide_by("a?b?c", "??", "a?b?c", "");
	test_divide_by("a?b??c", "??", "a?b", "c");
	test_divide_by("a??b?c", "??", "a", "b?c");
	test_divide_by("a??b?c", "?", "a", "?b?c");

	test_divide_by("this is awesome test case.", "awesome", "this is ", " test case.");
	test_divide_by("this is awesome test case.", "this", "", " is awesome test case.");
	test_divide_by("this is awesome test case.", "case.", "this is awesome test ", "");
	test_divide_by("this is awesome test case.", "case.", "this is awesome test ", "");

	ThinString              str("this is awesome test case.", 5, 8);
	ThinString::ThinStrPair p = str.DivideBy("some");
	EXPECT_EQ(p.first, "is aweso");
	EXPECT_EQ(p.second, "");
}

TEST(thin_string, MeasureUntil)
{
	ThinString str;

	str = ThinString("aaa/bbb");
	EXPECT_EQ(str.MeasureUntil("/"), 3);
	str = ThinString("aaa/bbb");
	EXPECT_EQ(str.MeasureUntil("bbbb"), 7);

	str = ThinString("aaa/bbb", 3, 10);
	EXPECT_EQ(str.MeasureUntil("/"), 0);
	str = ThinString("aaa/bbb", 4, 10);
	EXPECT_EQ(str.MeasureUntil("/"), str.len());

	str = ThinString("this is awesome test case.");
	EXPECT_EQ(str.MeasureUntil("this"), 0);
	str = ThinString("this is awesome test case.");
	EXPECT_EQ(str.MeasureUntil("awesome"), 8);

	str = ThinString("this is awesome test case.", 3, 10);
	EXPECT_EQ(str.MeasureUntil("this"), str.len());
	str = ThinString("this is awesome test case.", 3, 10);
	EXPECT_EQ(str.MeasureUntil("awesome"), str.len());
	str = ThinString("this is awesome test case.", 3, 10);
	EXPECT_EQ(str.MeasureUntil("case"), str.len());
}

TEST(thin_string, rfind_not_of)
{
	EXPECT_EQ(ThinString("012345").RFindNotOf("5"), 4);
	EXPECT_EQ(ThinString("012345").RFindNotOf("4"), 5);
	EXPECT_EQ(ThinString("012345").RFindNotOf("45"), 3);
	EXPECT_EQ(ThinString("012345").RFindNotOf("12345"), 0);
	EXPECT_EQ(ThinString("012345").RFindNotOf("01", 1), std::string::npos);
	EXPECT_EQ(ThinString("012345").RFindNotOf("01", 2), 2);
	EXPECT_EQ(ThinString("").RFindNotOf(""), std::string::npos);
	EXPECT_EQ(ThinString("").RFindNotOf("", 1), std::string::npos);
	EXPECT_EQ(ThinString("").RFindNotOf("", 2), std::string::npos);
	EXPECT_EQ(ThinString("").RFindNotOf("", 2), std::string::npos);
	EXPECT_EQ(ThinString("").RFindNotOf("0"), std::string::npos);
	EXPECT_EQ(ThinString("").RFindNotOf("0", 1), std::string::npos);
	EXPECT_EQ(ThinString("").RFindNotOf("0", 2), std::string::npos);
	EXPECT_EQ(ThinString("").RFindNotOf("0", 2), std::string::npos);
}

TEST(thin_string, r_itr)
{
	EXPECT_EQ(*ThinString("012345").rbegin(), '5');
	EXPECT_EQ(*++ThinString("012345").rbegin(), '4');
	EXPECT_EQ(*(ThinString("012345").rbegin() + 2), '3');
	EXPECT_EQ(*--ThinString("012345").rend(), '0');
	EXPECT_EQ(*(ThinString("012345").rend() - 2), '1');
}

TEST(thin_string, trim_left)
{
	EXPECT_EQ(ThinString("").TrimLeft(""), "");
	EXPECT_EQ(ThinString("abcdef012345").TrimLeft("abcdef"), "012345");
	EXPECT_EQ(ThinString("abc").TrimLeft("123"), "abc");
	EXPECT_EQ(ThinString("a").TrimLeft("a"), "");
	EXPECT_EQ(ThinString("aaab").TrimLeft("a"), "b");
	EXPECT_EQ(ThinString("aaab").TrimLeft("aa"), "b");
	EXPECT_EQ(ThinString("aaab").TrimLeft("aaa"), "b");
	EXPECT_EQ(ThinString("aaab").TrimLeft("ba"), "");
	EXPECT_EQ(ThinString("aaab").TrimLeft("b"), "aaab");
}

TEST(thin_string, trim_right)
{
	EXPECT_EQ(ThinString("").TrimRight(""), "");
	EXPECT_EQ(ThinString("abcdef012345").TrimRight("012345"), "abcdef");
	EXPECT_EQ(ThinString("abc").TrimRight("123"), "abc");
	EXPECT_EQ(ThinString("a").TrimRight("a"), "");
	EXPECT_EQ(ThinString("baaa").TrimRight("a"), "b");
	EXPECT_EQ(ThinString("baaa").TrimRight("aa"), "b");
	EXPECT_EQ(ThinString("baaa").TrimRight("aaa"), "b");
	EXPECT_EQ(ThinString("baaa").TrimRight("ba"), "");
	EXPECT_EQ(ThinString("baaa").TrimRight("ab"), "");
	EXPECT_EQ(ThinString("baaa").TrimRight("b"), "baaa");
}

TEST(thin_string, end_with)
{
	EXPECT_TRUE(ThinString("").EndWith(""));
	EXPECT_TRUE(ThinString("a").EndWith("a"));
	EXPECT_TRUE(ThinString("abc").EndWith("abc"));
	EXPECT_TRUE(ThinString("abc012").EndWith("012"));
	EXPECT_TRUE(ThinString("abcabc").EndWith("c"));
	EXPECT_TRUE(ThinString("a").EndWith(""));

	EXPECT_FALSE(ThinString("").EndWith("a"));
	EXPECT_FALSE(ThinString("abc").EndWith("a"));
	EXPECT_FALSE(ThinString("abc").EndWith("a"));
	EXPECT_FALSE(ThinString("abcabc").EndWith("ab"));
}

TEST(thin_string, start_with)
{
	EXPECT_TRUE(ThinString("").StartWith(""));
	EXPECT_TRUE(ThinString("a").StartWith("a"));
	EXPECT_TRUE(ThinString("abc").StartWith("abc"));
	EXPECT_TRUE(ThinString("abc").StartWith("a"));
	EXPECT_TRUE(ThinString("abc").StartWith("a"));
	EXPECT_TRUE(ThinString("abcabc").StartWith("ab"));
	EXPECT_TRUE(ThinString("a").StartWith(""));

	EXPECT_FALSE(ThinString("abc012").StartWith("012"));
	EXPECT_FALSE(ThinString("abcabc").StartWith("c"));
	EXPECT_FALSE(ThinString("").StartWith("a"));
	EXPECT_FALSE(ThinString("abcabc").StartWith("bc"));
}
