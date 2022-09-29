#include "gtest.h"
#include "origin_form.hpp"

#include "http_exceptions.hpp"

TEST(uri_parse, valid_origin_form)
{
	EXPECT_EQ(OriginForm("/"), OriginForm("/", ""));
	EXPECT_EQ(OriginForm("/index.html"), OriginForm("/index.html", ""));
	EXPECT_EQ(OriginForm("/?"), OriginForm("/", ""));
	EXPECT_EQ(OriginForm("/index.html?query"), OriginForm("/index.html", "query"));
	EXPECT_EQ(OriginForm("/index.html?query=1"), OriginForm("/index.html", "query=1"));
	EXPECT_EQ(
		OriginForm("/index.html?query=1&query=2"), OriginForm("/index.html", "query=1&query=2")
	);
}

TEST(uri_parse, invalid_origin_form)
{
	EXPECT_THROW(OriginForm(""), ParseErrorException);
	EXPECT_THROW(OriginForm("index.html"), ParseErrorException);
	EXPECT_THROW(OriginForm("index.html?query=1"), ParseErrorException);
	EXPECT_THROW(OriginForm("?query=1"), ParseErrorException);
	EXPECT_THROW(OriginForm("?"), ParseErrorException);
}
