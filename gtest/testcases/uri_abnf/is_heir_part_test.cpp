#include "gtest.h"

#include "uri_abnf.hpp"

// "//" authority path-abempty
TEST(uri_abnf, is_heir_part_authority_and_path_abempty)
{
	EXPECT_TRUE(ABNF::IsHeirPart("//"));
	EXPECT_TRUE(ABNF::IsHeirPart("//example.com"));
	EXPECT_TRUE(ABNF::IsHeirPart("//example.com/index.html"));
	EXPECT_TRUE(ABNF::IsHeirPart("//example.com//index.html"));

	EXPECT_TRUE(ABNF::IsHeirPart("//example.com:80//index.html"));

	EXPECT_TRUE(ABNF::IsHeirPart("//name@example.com//index.html"));
	EXPECT_TRUE(ABNF::IsHeirPart("//name:@example.com//index.html"));
	EXPECT_TRUE(ABNF::IsHeirPart("//name:pass@example.com//index.html"));
	EXPECT_TRUE(ABNF::IsHeirPart("//name:pass:@example.com//index.html"));
	EXPECT_TRUE(ABNF::IsHeirPart("//name:pass:xxxx@example.com//index.html"));
	EXPECT_TRUE(ABNF::IsHeirPart("//name:pass:xxxx@example.com:8080//index.html"));
}

// path-absolute = "/" [segment-nz *("/" segment )]
TEST(uri_abnf, is_heir_part_path_absolute)
{
	EXPECT_TRUE(ABNF::IsHeirPart("/example.com"));
	EXPECT_TRUE(ABNF::IsHeirPart("/example.com/"));
	EXPECT_TRUE(ABNF::IsHeirPart("/example.com/index.html"));
	EXPECT_TRUE(ABNF::IsHeirPart("/example.com//index.html"));
}

// path-rootless = segment-nz *("/" segment )
TEST(uri_abnf, is_heir_part_path_rootless)
{
	EXPECT_TRUE(ABNF::IsHeirPart("example.com"));
	EXPECT_TRUE(ABNF::IsHeirPart("example.com/"));
	EXPECT_TRUE(ABNF::IsHeirPart("example.com/index.html"));
	EXPECT_TRUE(ABNF::IsHeirPart("example.com//index.html"));
}

// path-empty = 0<pchar>
TEST(uri_abnf, is_heir_part_path_empty)
{
	EXPECT_TRUE(ABNF::IsHeirPart(""));
}

TEST(uri_abnf, is_heir_part_false_case)
{
	EXPECT_FALSE(ABNF::IsHeirPart("///"));
}
