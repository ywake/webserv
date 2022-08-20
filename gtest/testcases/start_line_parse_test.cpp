#include "http_message_headers.hpp"

#include "gtest.h"
#include <algorithm>
#include <cctype>

TEST(start_line, get)
{
	std::string input = "GET / HTTP/1.1\r\n\r\n";

	RequestMessage act;
	RequestMessage exp(
		RequestLine(
			RequestLine::GET,
			RequestTarget(RequestTarget::ORIGIN, URI("", "", "", "", "/", "", "")),
			"1.1"
		),
		FieldLines(),
		""
	);

	ASSERT_EQ(exp, act);
}
