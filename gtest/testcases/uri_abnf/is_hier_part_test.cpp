#include "gtest.h"

#include "uri_abnf.hpp"

// "//" authority path-abempty
TEST(uri_abnf, is_hier_part_authority_and_path_abempty)
{
	EXPECT_TRUE(ABNF::IsHierPart("//"));
	EXPECT_TRUE(ABNF::IsHierPart("//example.com"));
	EXPECT_TRUE(ABNF::IsHierPart("//example.com/index.html"));
	EXPECT_TRUE(ABNF::IsHierPart("//example.com//index.html"));

	EXPECT_TRUE(ABNF::IsHierPart("//example.com:80//index.html"));

	EXPECT_TRUE(ABNF::IsHierPart("//name@example.com//index.html"));
	EXPECT_TRUE(ABNF::IsHierPart("//name:@example.com//index.html"));
	EXPECT_TRUE(ABNF::IsHierPart("//name:pass@example.com//index.html"));
	EXPECT_TRUE(ABNF::IsHierPart("//name:pass:@example.com//index.html"));
	EXPECT_TRUE(ABNF::IsHierPart("//name:pass:xxxx@example.com//index.html"));
	EXPECT_TRUE(ABNF::IsHierPart("//name:pass:xxxx@example.com:8080//index.html"));
}

// path-absolute = "/" [segment-nz *("/" segment )]
TEST(uri_abnf, is_hier_part_path_absolute)
{
	EXPECT_TRUE(ABNF::IsHierPart("/example.com"));
	EXPECT_TRUE(ABNF::IsHierPart("/example.com/"));
	EXPECT_TRUE(ABNF::IsHierPart("/example.com/index.html"));
	EXPECT_TRUE(ABNF::IsHierPart("/example.com//index.html"));
}

// path-rootless = segment-nz *("/" segment )
TEST(uri_abnf, is_hier_part_path_rootless)
{
	EXPECT_FALSE(ABNF::IsHierPart("example.com"));
	EXPECT_FALSE(ABNF::IsHierPart("example.com/"));
	EXPECT_FALSE(ABNF::IsHierPart("example.com/index.html"));
	EXPECT_FALSE(ABNF::IsHierPart("example.com//index.html"));
}

// path-empty = 0<pchar>
TEST(uri_abnf, is_hier_part_path_empty)
{
	EXPECT_TRUE(ABNF::IsHierPart(""));
}

TEST(uri_abnf, is_hier_part_empty_authority_case)
{
	EXPECT_TRUE(ABNF::IsHierPart("///"));
	EXPECT_TRUE(ABNF::IsHierPart("///example.com"));
}
