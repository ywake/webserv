#include "gtest.h"
#include "http_message_headers.hpp"
#include <algorithm>
#include <cctype>

TEST(field_line, case_insensitive)
{
	FieldLines field;
	field["Content-Length"] = "11";
	EXPECT_EQ(field["content-length"], "11");
	field["Content-Length"] = "12";
	EXPECT_EQ(field["content-length"], "12");
}

/*
TEST(field_line, post_with_no_field_line)
{
	std::string input = "POST / HTTP/1.1\r\n\r\nHelloWorld !";

	RequestMessage act;

	RequestMessage exp(
		RequestLine(
			RequestLine::POST,
			RequestTarget(RequestTarget::ORIGIN, URI("", "", "", "", "/", "", "")),
			"1.1"
		),
		FieldLines(),
		""
	);

	ASSERT_EQ(act, exp);
}

TEST(field_line, post_with_single_field_line)
{
	std::string input = "POST / HTTP/1.1\r\nContent-Length: 11\r\nHelloWorld!";

	RequestMessage act;

	FieldLines field_lines;
	field_lines["Content-Length"] = "11";
	RequestMessage exp(
		RequestLine(
			RequestLine::POST,
			RequestTarget(RequestTarget::ORIGIN, URI("", "", "", "", "/", "", "")),
			"1.1"
		),
		field_lines,
		"HelloWorld!"
	);

	ASSERT_EQ(act, exp);
}

TEST(field_line, post_with_multi_field_lines)
{
	std::string input =
		"POST / HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 11\r\nHelloWorld!";

	RequestMessage act;

	FieldLines field_lines;
	field_lines["nConnection"] = "keep-alive";
	field_lines["Content-Length"] = "11";
	RequestMessage exp(
		RequestLine(
			RequestLine::POST,
			RequestTarget(RequestTarget::ORIGIN, URI("", "", "", "", "/", "", "")),
			"1.1"
		),
		field_lines,
		"HelloWorld!"
	);
	ASSERT_EQ(act, exp);
}
*/
