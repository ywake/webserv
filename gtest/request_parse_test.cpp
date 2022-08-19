#include "gtest.h"
#include <algorithm>
#include <cctype>

enum RequestForm {
	ORIGIN,
	ABSOLUTE,
	AUTHORITY,
	ASTERISK,
};

class URI
{
  public:
	std::string scheme_;
	std::string userinfo_;
	std::string host_;
	unsigned int port_;
	std::string path_;
	std::string query_;
	std::string fragment_;

	URI(std::string &scheme,
		std::string &userinfo,
		std::string &host,
		unsigned int &port,
		std::string &path,
		std::string &query,
		std::string &fragment)
		: scheme_(scheme), userinfo_(userinfo), host_(host), port_(port), path_(path),
		  query_(query), fragment_(fragment)
	{
	}
};

class RequestTarget
{
  public:
	RequestForm form_type_;
	URI request_target_;

	RequestTarget() : form_type_(), request_target_() {}

	RequestTarget(RequestForm form_type, URI &request_target)
		: form_type_(form_type), request_target_(request_target)
	{
	}
};

enum Methods {
	GET,
	POST,
	DELETE,
};

class RequestLine
{
  public:
	Methods method_;
	RequestTarget request_target_;
	std::string http_version_;

	RequestLine() : method_(), request_target_(), http_version_() {}

	RequestLine(Methods method, RequestTarget request_target, std::string http_version)
		: method_(method), request_target_(request_target), http_version_(http_version)
	{
	}
};

class StatusLine
{
  public:
	std::string http_version_;
	std::string status_code_;
	// std::string reason_phrase_;

	// std::string Str()
	// {
	// return http_version_ + SP + status_code_ +
	// SP + reason_phrase_;
	// }
};

class FieldLines
{
  private:
	std::map<const std::string, std::string> field_lines_;

  public:
	FieldLines() : field_lines_() {}

	std::string &operator[](std::string field_name)
	{
		for (std::string::iterator it = field_name.begin(); it != field_name.end(); it++) {
			*it = std::tolower(*it);
		}
		return field_lines_[field_name];
	}

	bool operator==(const FieldLines &rhs) const
	{
		return field_lines_ == rhs.field_lines_;
	}

	bool operator!=(const FieldLines &rhs) const
	{
		return !(*this == rhs);
	}
};

TEST(request_parse, field_line)
{
	FieldLines field;
	field["Content-Length"] = "11";
	EXPECT_EQ(field["content-length"], "11");
	field["Content-Length"] = "12";
	EXPECT_EQ(field["content-length"], "12");
}

class HTTPMessage
{
  public:
	FieldLines field_lines_;
	std::string message_body_;

	HTTPMessage(FieldLines field_lines, std::string message_body)
		: field_lines_(field_lines), message_body_(message_body)
	{
	}

	HTTPMessage(std::string request)
	{
		// TODO : parserは後で別関数に実装
	}

	bool operator==(const HTTPMessage &rhs) const
	{
		if (this->field_lines_ != rhs.field_lines_) {
			return false;
		}
		if (this->message_body_ != rhs.message_body_) {
			return false;
		}
		return true;
	}
};

class RequestMessage : public HTTPMessage
{
  public:
	RequestLine request_line_;

	RequestMessage(const std::string &str) : HTTPMessage(str)
	{
		// parse
	}

	RequestMessage(RequestLine &request_line, FieldLines &field_lines, std::string &message_body)
		: HTTPMessage(field_lines, message_body), request_line_(request_line)
	{
	}
};

class ResponseMessage : public HTTPMessage
{
  public:
	StatusLine status_line_;

	// ResponseMessage(StatusLine &status_line, FieldLines &field_lines, std::string &message_body)
	// : HTTPMessage(field_lines, message_body), status_line_(status_line)
	// {
	// }
}

TEST(request_parse, get)
{
	std::string input = "GET / HTTP/1.1\r\n\r\n";

	URI uri("", "", "", "", "/", "", "");
	RequestTarget target(ORIGIN, uri);
	RequestLine line(GET, target, "1.1");
	RequestMessage exp(line, FieldLines(), "");
	RequestMessage act(input);

	RequestMessage msg("GET / HTTP/1.1\r\n\r\n");

	ASSERT_EQ(exp, act);
}

// TEST(request_parse, post_with_no_field_line) {
// 	std::string input = "POST /
// HTTP/1.1\r\n\r\nHelloWorld!";

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

// TEST(request_parse,
// post_with_single_field_line) { 	std::string
// input = "POST / HTTP/1.1\r\nContent-Length:
// 11\r\nHelloWorld!";

// 	ASSERT_EQ(
// 		// actualy
// 		HTTPMessage(input),
// 		// expect
// 		HTTPMessage(
// 			"POST / HTTP/1.1",
// 			std::vector<std::string>{"Content-Length:
// 11"}, 			"HelloWorld!")
// 	);
// }

// TEST(request_parse,
// post_with_multi_field_lines) { 	std::string
// input = "POST / HTTP/1.1\r\nConnection:
// keep-alive\r\nContent-Length:
// 11\r\nHelloWorld!";

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
