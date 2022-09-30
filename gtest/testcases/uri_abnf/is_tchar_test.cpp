#include "gtest.h"
#include "validate_http_char.hpp"

TEST(uri_abnf, is_tchar_uniq)
{
	EXPECT_TRUE(http::abnf::IsTchar('!'));
	EXPECT_TRUE(http::abnf::IsTchar('#'));
	EXPECT_TRUE(http::abnf::IsTchar('$'));
	EXPECT_TRUE(http::abnf::IsTchar('%'));
	EXPECT_TRUE(http::abnf::IsTchar('&'));
	EXPECT_TRUE(http::abnf::IsTchar('\''));
	EXPECT_TRUE(http::abnf::IsTchar('*'));
	EXPECT_TRUE(http::abnf::IsTchar('+'));
	EXPECT_TRUE(http::abnf::IsTchar('-'));
	EXPECT_TRUE(http::abnf::IsTchar('.'));
	EXPECT_TRUE(http::abnf::IsTchar('^'));
	EXPECT_TRUE(http::abnf::IsTchar('_'));
	EXPECT_TRUE(http::abnf::IsTchar('`'));
	EXPECT_TRUE(http::abnf::IsTchar('|'));
	EXPECT_TRUE(http::abnf::IsTchar('~'));
}

TEST(uri_abnf, is_tchar_digit)
{
	for (int i = 0; i < 10; i++) {
		EXPECT_TRUE(http::abnf::IsTchar('0' + i));
	}
}

TEST(uri_abnf, is_tchar_alpha)
{
	for (int i = 0; i < 26; i++) {
		EXPECT_TRUE(http::abnf::IsTchar('a' + i));
	}
	for (int i = 0; i < 26; i++) {
		EXPECT_TRUE(http::abnf::IsTchar('A' + i));
	}
}

TEST(uri_abnf, is_tchar_uniq_false)
{
	EXPECT_FALSE(http::abnf::IsTchar('"'));
	EXPECT_FALSE(http::abnf::IsTchar('('));
	EXPECT_FALSE(http::abnf::IsTchar(')'));
	EXPECT_FALSE(http::abnf::IsTchar(','));
	EXPECT_FALSE(http::abnf::IsTchar('/'));
	EXPECT_FALSE(http::abnf::IsTchar(':'));
	EXPECT_FALSE(http::abnf::IsTchar(';'));
	EXPECT_FALSE(http::abnf::IsTchar('<'));
	EXPECT_FALSE(http::abnf::IsTchar('='));
	EXPECT_FALSE(http::abnf::IsTchar('>'));
	EXPECT_FALSE(http::abnf::IsTchar('?'));
	EXPECT_FALSE(http::abnf::IsTchar('@'));
	EXPECT_FALSE(http::abnf::IsTchar('['));
	EXPECT_FALSE(http::abnf::IsTchar('\\'));
	EXPECT_FALSE(http::abnf::IsTchar(']'));
	EXPECT_FALSE(http::abnf::IsTchar('{'));
	EXPECT_FALSE(http::abnf::IsTchar('}'));
	EXPECT_FALSE(http::abnf::IsTchar('\0'));
}

TEST(uri_abnf, is_tchar_only)
{
	EXPECT_TRUE(http::abnf::IsToken("!#$%&'*+-.^_`|~"));
	EXPECT_TRUE(http::abnf::IsToken("0123456789"));
	EXPECT_TRUE(http::abnf::IsToken("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	EXPECT_TRUE(http::abnf::IsToken(
		"!#$%&'*+-.^_`|~0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
	));
	EXPECT_TRUE(http::abnf::IsToken(
		"MIk.!uOjDGqH4d'$~m`|FYsJA&CPR^c%b2+59S7xwUK0fovVQ6a#*npZN-l_Bz13LrheTtWyXiEg8"
	));
	EXPECT_TRUE(http::abnf::IsToken(
		"3.75~0K*JGMZAieS94qc_UaB#p1oR!b'%dTLwOIjXn^m|&Fklh8Q-$+CxYf`t6EVszPvNDruWH2gy"
	));
}

TEST(uri_abnf, is_tchar_only_false)
{
	EXPECT_FALSE(http::abnf::IsToken(""));
	EXPECT_FALSE(http::abnf::IsToken("["));
	EXPECT_FALSE(http::abnf::IsToken("1]"));
	EXPECT_FALSE(http::abnf::IsToken("[1]"));
	EXPECT_FALSE(http::abnf::IsToken("0123456789\""));
	EXPECT_FALSE(http::abnf::IsToken("0123456789("));
	EXPECT_FALSE(http::abnf::IsToken("0123456789)"));
	EXPECT_FALSE(http::abnf::IsToken("0123456789,"));
	EXPECT_FALSE(http::abnf::IsToken("0123456789/"));
	EXPECT_FALSE(http::abnf::IsToken("0123456789:"));
	EXPECT_FALSE(http::abnf::IsToken("0123456789;"));
	EXPECT_FALSE(http::abnf::IsToken("0123456789<"));
	EXPECT_FALSE(http::abnf::IsToken("0123456789="));
	EXPECT_FALSE(http::abnf::IsToken("0123456789>"));
	EXPECT_FALSE(http::abnf::IsToken("0123456789?"));
	EXPECT_FALSE(http::abnf::IsToken("0123456789@"));
	EXPECT_FALSE(http::abnf::IsToken("0123456789["));
	EXPECT_FALSE(http::abnf::IsToken("0123456789\\"));
	EXPECT_FALSE(http::abnf::IsToken("0123456789]"));
	EXPECT_FALSE(http::abnf::IsToken("0123456789{"));
	EXPECT_FALSE(http::abnf::IsToken("0123456789}"));

	EXPECT_FALSE(http::abnf::IsToken("\"0123456789"));
	EXPECT_FALSE(http::abnf::IsToken("(0123456789"));
	EXPECT_FALSE(http::abnf::IsToken(")0123456789"));
	EXPECT_FALSE(http::abnf::IsToken(",0123456789"));
	EXPECT_FALSE(http::abnf::IsToken("/0123456789"));
	EXPECT_FALSE(http::abnf::IsToken(":0123456789"));
	EXPECT_FALSE(http::abnf::IsToken(";0123456789"));
	EXPECT_FALSE(http::abnf::IsToken("<0123456789"));
	EXPECT_FALSE(http::abnf::IsToken("=0123456789"));
	EXPECT_FALSE(http::abnf::IsToken(">0123456789"));
	EXPECT_FALSE(http::abnf::IsToken("?0123456789"));
	EXPECT_FALSE(http::abnf::IsToken("@0123456789"));
	EXPECT_FALSE(http::abnf::IsToken("[0123456789"));
	EXPECT_FALSE(http::abnf::IsToken("\\0123456789"));
	EXPECT_FALSE(http::abnf::IsToken("]0123456789"));
	EXPECT_FALSE(http::abnf::IsToken("{0123456789"));
	EXPECT_FALSE(http::abnf::IsToken("}0123456789"));

	EXPECT_FALSE(http::abnf::IsToken("01234\"56789\""));
	EXPECT_FALSE(http::abnf::IsToken("01234(56789("));
	EXPECT_FALSE(http::abnf::IsToken("01234)56789)"));
	EXPECT_FALSE(http::abnf::IsToken("01234,56789,"));
	EXPECT_FALSE(http::abnf::IsToken("01234/56789/"));
	EXPECT_FALSE(http::abnf::IsToken("01234:56789:"));
	EXPECT_FALSE(http::abnf::IsToken("01234;56789;"));
	EXPECT_FALSE(http::abnf::IsToken("01234<56789<"));
	EXPECT_FALSE(http::abnf::IsToken("01234=56789="));
	EXPECT_FALSE(http::abnf::IsToken("01234>56789>"));
	EXPECT_FALSE(http::abnf::IsToken("01234?56789?"));
	EXPECT_FALSE(http::abnf::IsToken("01234@56789@"));
	EXPECT_FALSE(http::abnf::IsToken("01234[56789["));
	EXPECT_FALSE(http::abnf::IsToken("01234\\56789\\"));
	EXPECT_FALSE(http::abnf::IsToken("01234]56789]"));
	EXPECT_FALSE(http::abnf::IsToken("01234{56789{"));
	EXPECT_FALSE(http::abnf::IsToken("01234}56789}"));

	EXPECT_FALSE(http::abnf::IsToken("\"0123\"456789"));
	EXPECT_FALSE(http::abnf::IsToken("(01234(56789"));
	EXPECT_FALSE(http::abnf::IsToken(")01234)56789"));
	EXPECT_FALSE(http::abnf::IsToken(",01234,56789"));
	EXPECT_FALSE(http::abnf::IsToken("/01234/56789"));
	EXPECT_FALSE(http::abnf::IsToken(":01234:56789"));
	EXPECT_FALSE(http::abnf::IsToken(";01234;56789"));
	EXPECT_FALSE(http::abnf::IsToken("<01234<56789"));
	EXPECT_FALSE(http::abnf::IsToken("=01234=56789"));
	EXPECT_FALSE(http::abnf::IsToken(">01234>56789"));
	EXPECT_FALSE(http::abnf::IsToken("?01234?56789"));
	EXPECT_FALSE(http::abnf::IsToken("@01234@56789"));
	EXPECT_FALSE(http::abnf::IsToken("[01234[56789"));
	EXPECT_FALSE(http::abnf::IsToken("\\0123\\456789"));
	EXPECT_FALSE(http::abnf::IsToken("]01234]56789"));
	EXPECT_FALSE(http::abnf::IsToken("{01234{56789"));
	EXPECT_FALSE(http::abnf::IsToken("}01234}56789"));

	EXPECT_FALSE(http::abnf::IsToken("\"0123\"456789\""));
	EXPECT_FALSE(http::abnf::IsToken("(01234(56789("));
	EXPECT_FALSE(http::abnf::IsToken(")01234)56789)"));
	EXPECT_FALSE(http::abnf::IsToken(",01234,56789,"));
	EXPECT_FALSE(http::abnf::IsToken("/01234/56789/"));
	EXPECT_FALSE(http::abnf::IsToken(":01234:56789:"));
	EXPECT_FALSE(http::abnf::IsToken(";01234;56789;"));
	EXPECT_FALSE(http::abnf::IsToken("<01234<56789<"));
	EXPECT_FALSE(http::abnf::IsToken("=01234=56789="));
	EXPECT_FALSE(http::abnf::IsToken(">01234>56789>"));
	EXPECT_FALSE(http::abnf::IsToken("?01234?56789?"));
	EXPECT_FALSE(http::abnf::IsToken("@01234@56789@"));
	EXPECT_FALSE(http::abnf::IsToken("[01234[56789["));
	EXPECT_FALSE(http::abnf::IsToken("\\0123\\456789\\"));
	EXPECT_FALSE(http::abnf::IsToken("]01234]56789]"));
	EXPECT_FALSE(http::abnf::IsToken("{01234{56789{"));
	EXPECT_FALSE(http::abnf::IsToken("}01234}56789}"));
}
