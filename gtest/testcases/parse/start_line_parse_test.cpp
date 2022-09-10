#include "error.hpp"
#include "http_message_headers.hpp"

#include "gtest.h"
#include "request_line.hpp"
#include <algorithm>
#include <cctype>

/*
TEST(start_line, get)
{
	std::string input = "GET / HTTP/1.1";

	RequestLine act(input);
	RequestLine exp(
		RequestLine::GET,
		RequestTarget(RequestTarget::ORIGIN, Uri("", "", "", "", "/", "", "")),
		"1.1"
	);

	ASSERT_EQ(exp, act);
}

TEST(start_line, whitespace)
{
	std::string input = "GET http: // HTTP/1.1";

	Error error;
	try {
		RequestLine act(input);
	} catch (const Error &e) {
		error = e;
	}
	ASSERT_EQ(error, Error("400"));
}
*/

TEST(request_line, error_case)
{
	EXPECT_THROW(RequestLine(""), Error);
	EXPECT_THROW(RequestLine(" "), Error);
	EXPECT_THROW(RequestLine("GET"), Error);
	EXPECT_THROW(RequestLine("GET "), Error);
	EXPECT_THROW(RequestLine("GET  "), Error);
	EXPECT_THROW(RequestLine("GET   "), Error);
	EXPECT_THROW(RequestLine(" GET"), Error);
	EXPECT_THROW(RequestLine(" GET "), Error);
	EXPECT_THROW(RequestLine(" GET  "), Error);
	EXPECT_THROW(RequestLine(" GET   "), Error);
	EXPECT_THROW(RequestLine("GET /"), Error);
	EXPECT_THROW(RequestLine("GET  /"), Error);
	EXPECT_THROW(RequestLine(" GET /"), Error);
	EXPECT_THROW(RequestLine("GET / "), Error);
	EXPECT_THROW(RequestLine("GET HTTP/1.1"), Error);
	EXPECT_THROW(RequestLine("GET  HTTP/1.1 "), Error);
	EXPECT_THROW(RequestLine("GET  HTTP/1.1"), Error);
	EXPECT_THROW(RequestLine("GET   HTTP/1.1"), Error);
	EXPECT_THROW(RequestLine("GET  HTTP/1.1 "), Error);
	EXPECT_THROW(RequestLine(" GET HTTP/1.1"), Error);
	EXPECT_THROW(RequestLine(" GET HTTP/1.1 "), Error);
	EXPECT_THROW(RequestLine(" GET  HTTP/1.1 "), Error);
	EXPECT_THROW(RequestLine("GET / HTTP/1.1 "), Error);
	EXPECT_THROW(RequestLine("GET  / HTTP/1.1"), Error);
	EXPECT_THROW(RequestLine("GET /  HTTP/1.1"), Error);
	EXPECT_THROW(RequestLine("GET / HTTP/1.1 "), Error);
	EXPECT_THROW(RequestLine(" GET / HTTP/1.1"), Error);
	EXPECT_THROW(RequestLine(" GET / HTTP/1.1 "), Error);
	EXPECT_THROW(RequestLine(" GET  / HTTP/1.1 "), Error);
	EXPECT_THROW(RequestLine(" GET  /  HTTP/1.1 "), Error);
	EXPECT_THROW(RequestLine(" GET /  HTTP/1.1 "), Error);
}
