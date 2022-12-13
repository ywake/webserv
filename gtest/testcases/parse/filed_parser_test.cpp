#include "field_parser.hpp"
#include "gtest.h"
#include "header_section.hpp"
#include "http_exceptions.hpp"

using namespace http;

q_buffer::QueuingBuffer   buf;
server::FieldParser       p;
Emptiable<FieldSection *> res;

void Parse(const std::string &s)
{
	delete res.Value();
	res = Emptiable<FieldSection *>();
	buf.push_back(s);
	res = p.Parse(buf);
}

TEST(field_parser, general)
{
	buf = q_buffer::QueuingBuffer();
	p   = server::FieldParser();

	EXPECT_EQ(p.Parse(buf), Emptiable<FieldSection *>());

	Parse("\r\n");
	EXPECT_EQ(*res.Value(), FieldSection());

	Parse("key: value\r\n"
		  "key2: value2\r\n"
		  " value2\r\n"
		  "\tvalue2\r\n");
	EXPECT_TRUE(res.empty());

	Parse("\r\n");
	EXPECT_EQ(
		*res.Value(),
		FieldSection(FieldSection::FieldLines(
			{{"key", {HeaderValue("value")}},
			 {"key2",
			  {HeaderValue("value2\r\n"
						   " value2\r\n"
						   "\tvalue2")}}}
		))
	);

	Parse("key: value\r");
	EXPECT_TRUE(res.empty());
	Parse("\n");
	EXPECT_TRUE(res.empty());
	Parse("key2: value2\r\n");
	EXPECT_TRUE(res.empty());
	Parse(" value2\r\n");
	EXPECT_TRUE(res.empty());
	Parse("\tvalue2\r\n\r\n");
	EXPECT_EQ(
		*res.Value(),
		FieldSection(FieldSection::FieldLines(
			{{"key", {HeaderValue("value")}},
			 {"key2",
			  {HeaderValue("value2\r\n"
						   " value2\r\n"
						   "\tvalue2")}}}
		))
	);

	Parse("a:\r\n");
	EXPECT_TRUE(res.empty());
	Parse("\r\n");
	EXPECT_EQ(*res.Value(), FieldSection(FieldSection::FieldLines({{"a", {HeaderValue("")}}})));

	Parse("a:");
	EXPECT_TRUE(res.empty());
	Parse("\r\n\r\n");
	EXPECT_EQ(*res.Value(), FieldSection(FieldSection::FieldLines({{"a", {HeaderValue("")}}})));

	Parse("key: value\r\n"
		  "key: value2  \r\n\r\n");
	EXPECT_EQ(
		*res.Value(),
		FieldSection(
			FieldSection::FieldLines({{"key", {HeaderValue("value"), HeaderValue("value2")}}})
		)
	);

	Parse("key: value\r\n"
		  " \r\n"
		  "\t\r\n\r\n");
	EXPECT_EQ(
		*res.Value(),
		FieldSection(FieldSection::FieldLines(
			{{"key",
			  {HeaderValue("value\r\n"
						   " \r\n"
						   "\t")}}}
		))
	);

	Parse("key: value\r\n"
		  " \r\n"
		  "\t\r\n\r\n");
	EXPECT_EQ(
		*res.Value(),
		FieldSection(FieldSection::FieldLines({{"key", {HeaderValue("value\r\n \r\n\t")}}}))
	);

	Parse("key: value\r\n"
		  "\r\n"
		  "\r\n");
	EXPECT_EQ(
		*res.Value(), FieldSection(FieldSection::FieldLines({{"key", {HeaderValue("value")}}}))
	);
	Parse("");
	EXPECT_EQ(*res.Value(), FieldSection());

	Parse("key: value\r\n\r\n");
	EXPECT_EQ(
		*res.Value(), FieldSection(FieldSection::FieldLines({{"key", {HeaderValue("value")}}}))
	);

	Parse("key: value\r\n\r\n");
	EXPECT_EQ(
		*res.Value(), FieldSection(FieldSection::FieldLines({{"key", {HeaderValue("value")}}}))
	);
	EXPECT_TRUE(buf.size() == 0);
}

TEST(field_parser, throw_test)
{
	buf = q_buffer::QueuingBuffer();
	p   = server::FieldParser();
	EXPECT_THROW(Parse(":\r\nh"), HttpException);

	buf = q_buffer::QueuingBuffer();
	EXPECT_THROW(Parse(" :\r\nh"), HttpException);

	buf = q_buffer::QueuingBuffer();
	EXPECT_THROW(
		Parse("key\r\n"
			  " : value\r\n\r\n"),
		HttpException
	);

	buf = q_buffer::QueuingBuffer();
	EXPECT_THROW(
		Parse("key\r\n"
			  "\t: value\r\n\r\n"),
		HttpException
	);

	buf = q_buffer::QueuingBuffer();
	EXPECT_THROW(
		Parse("key\r\n"
			  " : value\r\n\r\n"),
		HttpException
	);

	buf = q_buffer::QueuingBuffer();
	EXPECT_THROW(
		Parse("key\r\n"
			  "\t: value\r\n\r\n"),
		HttpException
	);
}
