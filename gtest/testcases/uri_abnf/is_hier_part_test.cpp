#include "gtest.h"

#include "hier_part.hpp"
#include "http_exceptions.hpp"
#include "parse_uri.hpp"

// "//" authority path-abempty
TEST(uri_abnf, hier_part_authority_and_path_abempty)
{
	EXPECT_EQ(HierPart("//example.com"), HierPart(Authority("", "example.com", ""), ""));
	EXPECT_EQ(
		HierPart("//example.com//index.html"),
		HierPart(Authority("", "example.com", ""), "//index.html")
	);
	EXPECT_EQ(
		HierPart("//example.com:80//index.html"),
		HierPart(Authority("", "example.com", "80"), "//index.html")
	);
	EXPECT_EQ(
		HierPart("//name@example.com//index.html"),
		HierPart(Authority("name", "example.com", ""), "//index.html")
	);
	EXPECT_EQ(
		HierPart("//name:@example.com//index.html"),
		HierPart(Authority("name:", "example.com", ""), "//index.html")
	);
	EXPECT_EQ(
		HierPart("//name:pass@example.com//index.html"),
		HierPart(Authority("name:pass", "example.com", ""), "//index.html")
	);
	EXPECT_EQ(
		HierPart("//name:pass:@example.com//index.html"),
		HierPart(Authority("name:pass:", "example.com", ""), "//index.html")
	);
	EXPECT_EQ(
		HierPart("//name:pass:xxxx@example.com//index.html"),
		HierPart(Authority("name:pass:xxxx", "example.com", ""), "//index.html")
	);
	EXPECT_EQ(
		HierPart("//name:pass:xxxx@example.com:8080//index.html"),
		HierPart(Authority("name:pass:xxxx", "example.com", "8080"), "//index.html")
	);
	EXPECT_EQ(
		HierPart("//example.com/index.html"),
		HierPart(Authority("", "example.com", ""), "/index.html")
	);
	EXPECT_EQ(HierPart("//"), HierPart(Authority("", "", ""), ""));

	EXPECT_THROW(HierPart("//example.com/\\"), http::BadRequestException);
	EXPECT_THROW(HierPart("//example.com/\""), http::BadRequestException);
}

// path-absolute = "/" [segment-nz *("/" segment )]
TEST(uri_abnf, hier_part_path_absolute)
{
	EXPECT_EQ(HierPart("/example.com"), HierPart(Authority(), "/example.com"));
	EXPECT_EQ(HierPart("/example.com/"), HierPart(Authority(), "/example.com/"));
	EXPECT_EQ(
		HierPart("/example.com/index.html"), HierPart(Authority(), "/example.com/index.html")
	);
	EXPECT_EQ(
		HierPart("/example.com//index.html"), HierPart(Authority(), "/example.com//index.html")
	);
}

// path-rootless = segment-nz *("/" segment )
TEST(uri_abnf, hier_part_path_rootless)
{
	EXPECT_EQ(HierPart("example.com"), HierPart(Authority(), "example.com"));
	EXPECT_EQ(HierPart("example.com/"), HierPart(Authority(), "example.com/"));
	EXPECT_EQ(HierPart("example.com/index.html"), HierPart(Authority(), "example.com/index.html"));
	EXPECT_EQ(
		HierPart("example.com//index.html"), HierPart(Authority(), "example.com//index.html")
	);
}

// path-empty = 0<pchar>
TEST(uri_abnf, hier_part_path_empty)
{
	EXPECT_EQ(HierPart(""), HierPart(Authority(), ""));
}

TEST(uri_abnf, hier_part_empty_authority_case)
{
	EXPECT_EQ(HierPart("///"), HierPart(Authority(), "/"));
	EXPECT_EQ(HierPart("///example.com"), HierPart(Authority(), "/example.com"));
}
