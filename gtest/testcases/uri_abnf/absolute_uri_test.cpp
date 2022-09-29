#include "absolute_uri.hpp"
#include "gtest.h"

#include "http_exceptions.hpp"

// URI = scheme ":" hier-part [ "?" query ]
TEST(absolute_uri, ok)
{
	// "//"" authorithy path-abempty
	EXPECT_EQ(
		AbsoluteUri("http://www.google.com/"),
		AbsoluteUri("http", HierPart("//www.google.com/"), "")
	);
	// path-absolute
	EXPECT_EQ(AbsoluteUri("scheme:/"), AbsoluteUri("scheme", HierPart("/"), ""));
	// path-rootless
	EXPECT_EQ(
		AbsoluteUri("file:user@host:80/path?query"),
		AbsoluteUri("file", HierPart("user@host:80/path"), "query")
	);
	// path-empty
	EXPECT_EQ(AbsoluteUri("scheme:"), AbsoluteUri("scheme", HierPart(""), ""));
}

TEST(absolute_uri, invalid)
{
	// no : after scheme
	EXPECT_THROW(AbsoluteUri(""), BadRequestException);
	EXPECT_THROW(AbsoluteUri("a"), BadRequestException);
	EXPECT_THROW(AbsoluteUri("http//www.google.com/"), BadRequestException);
	// no-scheme
	EXPECT_THROW(AbsoluteUri("://www.google.com/"), BadRequestException);
}
