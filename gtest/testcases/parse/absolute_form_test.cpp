#include "absolute_form.hpp"
#include "gtest.h"

#include "error.hpp"

TEST(uri_parse, valid_absolute_form)
{
	EXPECT_EQ(
		AbsoluteForm("http://example.com"),
		AbsoluteForm(AbsoluteUri("http", HierPart(Authority("", "example.com", ""), ""), ""))
	);
	EXPECT_EQ(
		AbsoluteForm("https://example.com"),
		AbsoluteForm(AbsoluteUri("https", HierPart(Authority("", "example.com", ""), ""), ""))
	);
	EXPECT_EQ(
		AbsoluteForm("http://example.com/"),
		AbsoluteForm(AbsoluteUri("http", HierPart(Authority("", "example.com", ""), "/"), ""))
	);
	EXPECT_EQ(
		AbsoluteForm("http://example.com/index.html"),
		AbsoluteForm(
			AbsoluteUri("http", HierPart(Authority("", "example.com", ""), "/index.html"), "")
		)
	);
	EXPECT_EQ(
		AbsoluteForm("http://example.com/index.html?query=1"),
		AbsoluteForm(AbsoluteUri(
			"http", HierPart(Authority("", "example.com", ""), "/index.html"), "query=1"
		))
	);
	EXPECT_EQ(
		AbsoluteForm("http://example.com/index.html?query=1&query=2"),
		AbsoluteForm(AbsoluteUri(
			"http", HierPart(Authority("", "example.com", ""), "/index.html"), "query=1&query=2"
		))
	);
	EXPECT_EQ(
		AbsoluteForm("http://example.com:8080"),
		AbsoluteForm(AbsoluteUri("http", HierPart(Authority("", "example.com", "8080"), ""), ""))
	);
	EXPECT_EQ(
		AbsoluteForm("http://example.com:8080/index.html"),
		AbsoluteForm(
			AbsoluteUri("http", HierPart(Authority("", "example.com", "8080"), "/index.html"), "")
		)
	);
	EXPECT_EQ(
		AbsoluteForm("http://username:password@example.com:8080/index.html?query=1"),
		AbsoluteForm(AbsoluteUri(
			"http",
			HierPart(Authority("username:password", "example.com", "8080"), "/index.html"),
			"query=1"
		))
	);
	EXPECT_EQ(
		AbsoluteForm("http://username:password:@example.com:8080/index.html?query=1"),
		AbsoluteForm(AbsoluteUri(
			"http",
			HierPart(Authority("username:password:", "example.com", "8080"), "/index.html"),
			"query=1"
		))
	);
}

TEST(uri_parse, invalid_absolute_form)
{
	// no-host
	EXPECT_THROW(AbsoluteForm("http://"), Error);
	EXPECT_THROW(AbsoluteForm("http:///index.html"), Error);
	EXPECT_THROW(AbsoluteForm("http://:80"), Error);
	EXPECT_THROW(AbsoluteForm("http://username:password@:80"), Error);

	// no-scheme
	EXPECT_THROW(AbsoluteForm("://example.com"), Error);
	EXPECT_THROW(AbsoluteForm("://username:password@example.com:80/index.html?query"), Error);

	// no-scheme and no-host
	EXPECT_THROW(AbsoluteForm("://"), Error);

	// scheme is not http or https
	EXPECT_THROW(AbsoluteForm("ftp://example.com"), Error);

	// hierpart path-rootless
	EXPECT_THROW(AbsoluteForm("http:path/index.html"), Error);
}
