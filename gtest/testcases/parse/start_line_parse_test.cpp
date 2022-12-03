#include "http_exceptions.hpp"

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
	ASSERT_EQ(error, BadRequestException());
}
*/

TEST(request_line, ok_case)
{
	EXPECT_EQ(RequestLine("GET / HTTP/1.1"), RequestLine("GET", OriginForm("/"), "HTTP/1.1"));
	EXPECT_EQ(RequestLine("GET / HTTP/4.2"), RequestLine("GET", OriginForm("/"), "HTTP/4.2"));
	EXPECT_EQ(RequestLine("POST / HTTP/1.1"), RequestLine("POST", OriginForm("/"), "HTTP/1.1"));
	EXPECT_EQ(RequestLine("DELETE / HTTP/1.1"), RequestLine("DELETE", OriginForm("/"), "HTTP/1.1"));
	EXPECT_EQ(
		RequestLine("OPTIONS / HTTP/1.1"), RequestLine("OPTIONS", OriginForm("/"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("GET http://a/b/ HTTP/1.1"),
		RequestLine("GET", AbsoluteForm("http://a/b/"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("GET http://a/b HTTP/1.1"),
		RequestLine("GET", AbsoluteForm("http://a/b"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("POST http://a/b HTTP/1.1"),
		RequestLine("POST", AbsoluteForm("http://a/b"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("DELETE http://a/b HTTP/1.1"),
		RequestLine("DELETE", AbsoluteForm("http://a/b"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("OPTIONS http://a/b HTTP/1.1"),
		RequestLine("OPTIONS", AbsoluteForm("http://a/b"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("CONNECT a:1 HTTP/1.1"),
		RequestLine("CONNECT", AuthorityForm("a:1"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("OPTIONS * HTTP/1.1"), RequestLine("OPTIONS", AsteriskForm("*"), "HTTP/1.1")
	);
}

TEST(request_line, relative_case)
{
	EXPECT_EQ(
		RequestLine("GET http://a/b/../ HTTP/1.1"),
		RequestLine("GET", AbsoluteForm("http://a/"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("GET http://a/../b HTTP/1.1"),
		RequestLine("GET", AbsoluteForm("http://a/b"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("GET http://a/b/../ HTTP/1.1"),
		RequestLine("GET", AbsoluteForm("http://a/"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("GET http://a/../b HTTP/1.1"),
		RequestLine("GET", AbsoluteForm("http://a/b"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("GET /a/b/../ HTTP/1.1"), RequestLine("GET", OriginForm("/a/"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("GET /a/../b HTTP/1.1"), RequestLine("GET", OriginForm("/b"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("GET /a/b/../ HTTP/1.1"), RequestLine("GET", OriginForm("/a/"), "HTTP/1.1")
	);
	EXPECT_EQ(
		RequestLine("GET /a/../b HTTP/1.1"), RequestLine("GET", OriginForm("/b"), "HTTP/1.1")
	);
}

TEST(request_line, error_case)
{
	EXPECT_THROW(RequestLine(""), http::BadRequestException);
	EXPECT_THROW(RequestLine(" "), http::BadRequestException);
	EXPECT_THROW(RequestLine("  "), http::BadRequestException);
	EXPECT_THROW(RequestLine("   "), http::BadRequestException);
	EXPECT_THROW(RequestLine("    "), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET"), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET "), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET  "), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET   "), http::BadRequestException);
	EXPECT_THROW(RequestLine(" GET"), http::BadRequestException);
	EXPECT_THROW(RequestLine(" GET "), http::BadRequestException);
	EXPECT_THROW(RequestLine(" GET  "), http::BadRequestException);
	EXPECT_THROW(RequestLine(" GET   "), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET /"), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET  /"), http::BadRequestException);
	EXPECT_THROW(RequestLine(" GET /"), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET / "), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET HTTP/1.1"), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET  HTTP/1.1 "), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET  HTTP/1.1"), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET   HTTP/1.1"), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET  HTTP/1.1 "), http::BadRequestException);
	EXPECT_THROW(RequestLine(" GET HTTP/1.1"), http::BadRequestException);
	EXPECT_THROW(RequestLine(" GET HTTP/1.1 "), http::BadRequestException);
	EXPECT_THROW(RequestLine(" GET  HTTP/1.1 "), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET / HTTP/1.1 "), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET  / HTTP/1.1"), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET /  HTTP/1.1"), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET / HTTP/1.1 "), http::BadRequestException);
	EXPECT_THROW(RequestLine(" GET / HTTP/1.1"), http::BadRequestException);
	EXPECT_THROW(RequestLine(" GET / HTTP/1.1 "), http::BadRequestException);
	EXPECT_THROW(RequestLine(" GET  / HTTP/1.1 "), http::BadRequestException);
	EXPECT_THROW(RequestLine(" GET  /  HTTP/1.1 "), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET/HTTP/1.1"), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET_/_HTTP/1.1"), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET / HTTP/42.42"), http::BadRequestException);
	EXPECT_THROW(RequestLine("GET / HTTP/1.1\r\n"), http::BadRequestException);
}
