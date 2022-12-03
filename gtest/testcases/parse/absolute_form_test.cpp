#include "absolute_form.hpp"
#include "gtest.h"

#include "http_exceptions.hpp"

TEST(uri_parse, valid_absolute_form)
{
	EXPECT_EQ(
		AbsoluteForm("http://example.com"),
		AbsoluteForm(AbsoluteUri("http", HierPart(Authority("", "example.com", ""), ""), ""))
	);
	EXPECT_EQ(
		AbsoluteForm("hTtp://example.com"),
		AbsoluteForm(AbsoluteUri("hTtp", HierPart(Authority("", "example.com", ""), ""), ""))
	);
	EXPECT_EQ(
		AbsoluteForm("HTTP://example.com"),
		AbsoluteForm(AbsoluteUri("HTTP", HierPart(Authority("", "example.com", ""), ""), ""))
	);
	EXPECT_EQ(
		AbsoluteForm("Http://example.com"),
		AbsoluteForm(AbsoluteUri("Http", HierPart(Authority("", "example.com", ""), ""), ""))
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
	EXPECT_THROW(AbsoluteForm("http://"), http::BadRequestException);
	EXPECT_THROW(AbsoluteForm("http:///index.html"), http::BadRequestException);
	EXPECT_THROW(AbsoluteForm("http://:80"), http::BadRequestException);
	EXPECT_THROW(AbsoluteForm("http://username:password@:80"), http::BadRequestException);

	// no-scheme
	EXPECT_THROW(AbsoluteForm("://example.com"), http::BadRequestException);
	EXPECT_THROW(
		AbsoluteForm("://username:password@example.com:80/index.html?query"),
		http::BadRequestException
	);

	// no-scheme and no-host
	EXPECT_THROW(AbsoluteForm("://"), http::BadRequestException);

	// scheme is not http
	EXPECT_THROW(AbsoluteForm("ftp://example.com"), http::BadRequestException);
	EXPECT_THROW(AbsoluteForm("https://example.com"), http::BadRequestException);

	// hierpart path-rootless
	EXPECT_THROW(AbsoluteForm("http:path/index.html"), http::BadRequestException);
}
