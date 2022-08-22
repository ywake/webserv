#include "gtest.h"

#include "uri_abnf.hpp"

// "%" HEXDIG HEXDIG
TEST(uri_abnf, IsPctEncoded_hex)
{
	ASSERT_TRUE(ABNF::IsPctEncoded("%00"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%11"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%22"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%33"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%44"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%55"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%66"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%77"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%88"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%99"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%AA"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%BB"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%CC"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%DD"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%EE"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%FF"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%aa"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%bb"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%cc"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%dd"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%ee"));
	ASSERT_TRUE(ABNF::IsPctEncoded("%ff"));
}

TEST(uri_abnf, IsPctEncoded_not_hex)
{
	ASSERT_FALSE(ABNF::IsPctEncoded("%0G"));
	ASSERT_FALSE(ABNF::IsPctEncoded("%h0"));
	ASSERT_FALSE(ABNF::IsPctEncoded("%0!"));
	ASSERT_FALSE(ABNF::IsPctEncoded("%0:"));
	ASSERT_FALSE(ABNF::IsPctEncoded("%0`"));
	ASSERT_FALSE(ABNF::IsPctEncoded("%0~"));
}

TEST(uri_abnf, IsPctEncoded_start_with_no_pct)
{
	ASSERT_FALSE(ABNF::IsPctEncoded("0%0"));
	ASSERT_FALSE(ABNF::IsPctEncoded("00%"));
	ASSERT_FALSE(ABNF::IsPctEncoded("00"));
	ASSERT_FALSE(ABNF::IsPctEncoded("00A"));
}