#include "gtest.h"

enum Methods {
	GET,
	POST,
	DELET,
};

class RequestLine {
	public:
	Methods method_;
	std::string request_target_;
	std::string http_version_num_;

	std::string http_version() {
		return "HTTP/" + http_version_num_;
	}
};

class StatusLine {

};

class StartLine {
	public:
	RequestLine request_line_;
	StatusLine status_line_;
};

class HTTPMessage {
	public:
	std::string start_line_;
	std::map<std::string, std::string> field_lines_;
	std::string message_body_;

	// HTTPMessage(std::string start_line, std::map<std::string, std::string> field_lines, std::string message_body) : start_line_(start_line), field_lines_(field_lines), message_body_(message_body) {
	// }

	// HTTPMessage(std::string request) {
	// 	// TODO: parserは後で別関数に実装
	// }

	// bool operator==(const HTTPMessage &rhs) const {
	// 	if (this->start_line_ != rhs.start_line_) {
	// 		return false;
	// 	}
	// 	if (this->field_lines_ != rhs.field_lines_) {
	// 		return false;
	// 	}
	// 	if (this->message_body_ != rhs.message_body_) {
	// 		return false;
	// 	}
	// 	return true;
	// }
};

// TEST(request_parse, get) {
// 	std::string input = "GET / HTTP/1.1\r\n\r\n";

// 	ASSERT_EQ(
// 		// actualy
// 		HTTPMessage(input),
// 		// expect
// 		HTTPMessage(
// 			"GET / HTTP/1.1",
// 			std::vector<std::string>(),
// 			"")
// 	);
// }

// TEST(request_parse, post_with_no_field_line) {
// 	std::string input = "POST / HTTP/1.1\r\n\r\nHelloWorld!";

// 	ASSERT_EQ(
// 		// actualy
// 		HTTPMessage(input),
// 		// expect
// 		HTTPMessage(
// 			"POST / HTTP/1.1",
// 			std::vector<std::string>(),
// 			"HelloWorld!")
// 	);
// }

// TEST(request_parse, post_with_single_field_line) {
// 	std::string input = "POST / HTTP/1.1\r\nContent-Length: 11\r\nHelloWorld!";

// 	ASSERT_EQ(
// 		// actualy
// 		HTTPMessage(input),
// 		// expect
// 		HTTPMessage(
// 			"POST / HTTP/1.1",
// 			std::vector<std::string>{"Content-Length: 11"},
// 			"HelloWorld!")
// 	);
// }

// TEST(request_parse, post_with_multi_field_lines) {
// 	std::string input = "POST / HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 11\r\nHelloWorld!";

// 	ASSERT_EQ(
// 		// actualy
// 		HTTPMessage(input),
// 		// expect
// 		HTTPMessage(
// 			"POST / HTTP/1.1",
// 			std::vector<std::string>{
// 				"Connection: keep-alive",
// 				"Content-Length: 11"
// 			},
// 			"HelloWorld!")
// 	);
// }

TEST(HTTPmessage, 2) {
	ASSERT_EQ(1, 1);
}
