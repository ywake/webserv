#include "gtest.h"

#include <list>

#include "header_section.hpp"
#include "http_exceptions.hpp"

TEST(header_section, general)
{
	EXPECT_EQ(
		HeaderSection("key: value\r\n"
					  "key2: value2\r\n"
					  " value2\r\n"
					  "\tvalue2\r\n"),
		HeaderSection(HeaderSection::Headers(
			{{"key", {HeaderValue("value")}},
			 {"key2",
			  {HeaderValue("value2\r\n"
						   " value2\r\n"
						   "\tvalue2")}}}
		))
	);

	EXPECT_EQ(
		HeaderSection("a:\r\n"), HeaderSection(HeaderSection::Headers({{"a", {HeaderValue("")}}}))
	);
	EXPECT_EQ(
		HeaderSection("key: value\r\n"
					  "key: value2  \r\n"),
		HeaderSection(
			HeaderSection::Headers({{"key", {HeaderValue("value"), HeaderValue("value2")}}})
		)
	);
	EXPECT_EQ(
		HeaderSection("key: value\r\n"
					  " \r\n"
					  "\t\r\n"),
		HeaderSection(HeaderSection::Headers(
			{{"key",
			  {HeaderValue("value\r\n"
						   " \r\n"
						   "\t")}}}
		))
	);
	EXPECT_EQ(
		HeaderSection("key: value\r\n"
					  " \r\n"
					  "\t\r\n"),
		HeaderSection(HeaderSection::Headers({{"key", {HeaderValue("value\r\n \r\n\t")}}}))
	);
}

TEST(header_section, throw_test)
{
	EXPECT_THROW(HeaderSection(":"), http::BadRequestException);
	EXPECT_THROW(HeaderSection(" :"), http::BadRequestException);
	EXPECT_THROW(HeaderSection("\r\n"), http::BadRequestException);
	EXPECT_THROW(HeaderSection("a:"), http::BadRequestException);
	EXPECT_THROW(HeaderSection("key: value\r\n\r\n"), http::BadRequestException);
	EXPECT_THROW(
		HeaderSection("key: value\r\n"
					  "\r\n"
					  "\r\n"),
		http::BadRequestException
	);
	EXPECT_THROW(
		HeaderSection("key\r\n"
					  " : value\r\n"),
		http::BadRequestException
	);
	EXPECT_THROW(
		HeaderSection("key\r\n"
					  "\t: value\r\n"),
		http::BadRequestException
	);
	EXPECT_THROW(
		HeaderSection("key\r\n"
					  " : value"),
		http::BadRequestException
	);
	EXPECT_THROW(
		HeaderSection("key\r\n"
					  "\t: value"),
		http::BadRequestException
	);
	EXPECT_THROW(
		HeaderSection("key: value\r\n"
					  " \r\n"
					  "\t"),
		http::BadRequestException
	);
	EXPECT_THROW(
		HeaderSection("key: value\r\n"
					  " \r\n"
					  " "),
		http::BadRequestException
	);
}

TEST(header_section, host)
{
	EXPECT_EQ(
		HeaderSection("host: localhost\r\n"),
		HeaderSection(HeaderSection::Headers({{"host", {HeaderValue("localhost")}}}))
	);
	EXPECT_EQ(
		HeaderSection("Host: localhost\r\n"),
		HeaderSection(HeaderSection::Headers({{"host", {HeaderValue("localhost")}}}))
	);
	EXPECT_EQ(
		HeaderSection("Host: localhost:8080\r\n"),
		HeaderSection(HeaderSection::Headers({{"host", {HeaderValue("localhost:8080")}}}))
	);
	EXPECT_EQ(
		HeaderSection("Host: localhost:\r\n"),
		HeaderSection(HeaderSection::Headers({{"host", {HeaderValue("localhost:")}}}))
	);

	// error case
	EXPECT_THROW(HeaderSection("Host: \r\n"), http::BadRequestException);
	EXPECT_THROW(HeaderSection("Host: localhost:8080:8080\r\n"), http::BadRequestException);
	EXPECT_THROW(HeaderSection("Host: localhost, example\r\n"), http::BadRequestException);
}

TEST(header_section, content_length)
{
	EXPECT_EQ(
		HeaderSection("content-length:11\r\n"),
		HeaderSection(HeaderSection::Headers({{"content-length", {HeaderValue("11")}}}))
	);
	EXPECT_EQ(
		HeaderSection("Content-Length:11\r\n"),
		HeaderSection(HeaderSection::Headers({{"content-length", {HeaderValue("11")}}}))
	);

	// error case
	EXPECT_THROW(HeaderSection("Content-Length:\r\n"), http::BadRequestException);
}

TEST(header_section, connection)
{
	EXPECT_EQ(
		HeaderSection("connection: close\r\n"),
		HeaderSection(HeaderSection::Headers({{"connection", {HeaderValue("close")}}}))
	);
	EXPECT_EQ(
		HeaderSection("Connection:close\r\n"),
		HeaderSection(HeaderSection::Headers({{"connection", {HeaderValue("close")}}}))
	);
	EXPECT_EQ(
		HeaderSection("Connection: close, aaa,bbb\r\n"),
		HeaderSection(HeaderSection::Headers(
			{{"connection", {HeaderValue("close"), HeaderValue("aaa"), HeaderValue("bbb")}}}
		))
	);

	// error case
	EXPECT_THROW(HeaderSection("Connection:\r\n"), http::BadRequestException);
	EXPECT_THROW(HeaderSection("Connection: close,\r\n"), http::BadRequestException);
	EXPECT_THROW(HeaderSection("Connection: close,,,,aaa\r\n"), http::BadRequestException);
	EXPECT_THROW(HeaderSection("Connection: a@b\r\n"), http::BadRequestException);
}

TEST(header_section, transfer_encoding)
{
	EXPECT_EQ(
		HeaderSection("transfer-encoding: chunked\r\n"),
		HeaderSection(HeaderSection::Headers({{"transfer-encoding", {HeaderValue("chunked")}}}))
	);
	EXPECT_EQ(
		HeaderSection("Transfer-Encoding: gzip, chunked\r\n"),
		HeaderSection(HeaderSection::Headers(
			{{"transfer-encoding", {HeaderValue("gzip"), HeaderValue("chunked")}}}
		))
	);
	EXPECT_EQ(
		HeaderSection("Transfer-Encoding: gzip , chunked\r\n"),
		HeaderSection(HeaderSection::Headers(
			{{"transfer-encoding", {HeaderValue("gzip"), HeaderValue("chunked")}}}
		))
	);
	EXPECT_EQ(
		HeaderSection("Transfer-Encoding: gzip,chunked\r\n"),
		HeaderSection(HeaderSection::Headers(
			{{"transfer-encoding", {HeaderValue("gzip"), HeaderValue("chunked")}}}
		))
	);
	EXPECT_EQ(
		HeaderSection("Transfer-Encoding: gzip ,chunked\r\n"),
		HeaderSection(HeaderSection::Headers(
			{{"transfer-encoding", {HeaderValue("gzip"), HeaderValue("chunked")}}}
		))
	);

	// error case
	EXPECT_THROW(HeaderSection("Transfer-Encoding:\r\n"), http::NotImplementedException);
	// error case
	EXPECT_THROW(HeaderSection("Transfer-Encoding: chunked,\r\n"), http::NotImplementedException);
}
