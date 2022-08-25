#include "error.hpp"
#include "http_message_headers.hpp"

#include "gtest.h"
#include <algorithm>
#include <cctype>

TEST(start_line, get)
{
	std::string input = "GET / HTTP/1.1";

	RequestLine act(input);
	RequestLine exp(
		RequestLine::GET,
		RequestTarget(RequestTarget::ORIGIN, URI("", "", "", "", "/", "", "")),
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
