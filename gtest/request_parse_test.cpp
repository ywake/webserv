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
	std::string port_;
	std::string path_;
	std::string query_;
	std::string fragment_;

	URI() : scheme_(), userinfo_(), host_(), port_(), path_(), query_(), fragment_() {}

	URI(std::string &scheme,
		std::string &userinfo,
		std::string &host,
		std::string &port,
		std::string &path,
		std::string &query,
		std::string &fragment)
		: scheme_(scheme), userinfo_(userinfo), host_(host), port_(port), path_(path),
		  query_(query), fragment_(fragment)
	{
	}

	URI(const char *scheme,
		const char *userinfo,
		const char *host,
		const char *port,
		const char *path,
		const char *query,
		const char *fragment)
		: scheme_(scheme), userinfo_(userinfo), host_(host), port_(port), path_(path),
		  query_(query), fragment_(fragment)
	{
	}

	bool operator==(const URI &rhs) const
	{
		return scheme_ == rhs.scheme_ && userinfo_ == rhs.userinfo_ && host_ == rhs.host_ &&
			   port_ == rhs.port_ && path_ == rhs.path_ && query_ == rhs.query_ &&
			   fragment_ == rhs.fragment_;
	}
};

class RequestTarget
{
  public:
	RequestForm form_type_;
	URI request_target_; // TODO: variable name

	RequestTarget() : form_type_(), request_target_() {}

	RequestTarget(RequestForm form_type, URI request_target)
		: form_type_(form_type), request_target_(request_target)
	{
	}

	bool operator==(const RequestTarget &rhs) const
	{
		return form_type_ == rhs.form_type_ && request_target_ == rhs.request_target_;
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

	bool operator==(const RequestLine &rhs) const
	{
		return method_ == rhs.method_ && request_target_ == rhs.request_target_ &&
			   http_version_ == rhs.http_version_;
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
	bool operator==(const StatusLine &rhs) const
	{
		return http_version_ == rhs.http_version_ && status_code_ == rhs.status_code_;
	}
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
		return field_lines_ == rhs.field_lines_ && message_body_ == rhs.message_body_;
	}
};

class RequestMessage : public HTTPMessage
{
  public:
	RequestLine request_line_;

	RequestMessage() : HTTPMessage(FieldLines(), "") {}

	RequestMessage(const std::string &str) : HTTPMessage(str)
	{
		// parse
	}

	RequestMessage(RequestLine request_line, FieldLines field_lines, std::string message_body)
		: HTTPMessage(field_lines, message_body), request_line_(request_line)
	{
	}

	bool operator==(const RequestMessage &rhs) const
	{
		return HTTPMessage::operator==(rhs) && request_line_ == rhs.request_line_;
	}
};

class ResponseMessage : public HTTPMessage
{
  public:
	StatusLine status_line_;

	ResponseMessage(StatusLine &status_line, FieldLines &field_lines, std::string &message_body)
		: HTTPMessage(field_lines, message_body), status_line_(status_line)
	{
	}

	bool operator==(const ResponseMessage &rhs) const
	{
		return HTTPMessage::operator==(rhs) && status_line_ == rhs.status_line_;
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

TEST(request_parse, get)
{
	std::string input = "GET / HTTP/1.1\r\n\r\n";

	RequestMessage act;
	RequestMessage exp(
		RequestLine(
			Methods::GET,
			RequestTarget(RequestForm::ORIGIN, URI("", "", "", "", "/", "", "")),
			"1.1"
		),
		FieldLines(),
		""
	);

	ASSERT_EQ(exp, act);
}

TEST(request_parse, post_with_no_field_line)
{
	std::string input = "POST / HTTP/1.1\r\n\r\nHelloWorld !";

	RequestMessage act;

	RequestMessage exp(
		RequestLine(
			Methods::POST,
			RequestTarget(RequestForm::ORIGIN, URI("", "", "", "", "/", "", "")),
			"1.1"
		),
		FieldLines(),
		""
	);

	ASSERT_EQ(act, exp);
}

TEST(request_parse, post_with_single_field_line)
{
	std::string input = "POST / HTTP/1.1\r\nContent-Length: 11\r\nHelloWorld!";

	RequestMessage act;

	FieldLines field_lines;
	field_lines["Content-Length"] = "11";
	RequestMessage exp(
		RequestLine(
			Methods::POST,
			RequestTarget(RequestForm::ORIGIN, URI("", "", "", "", "/", "", "")),
			"1.1"
		),
		field_lines,
		"HelloWorld!"
	);

	ASSERT_EQ(act, exp);
}

TEST(request_parse, post_with_multi_field_lines)
{
	std::string input =
		"POST / HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 11\r\nHelloWorld!";

	RequestMessage act;

	FieldLines field_lines;
	field_lines["nConnection"] = "keep-alive";
	field_lines["Content-Length"] = "11";
	RequestMessage exp(
		RequestLine(
			Methods::POST,
			RequestTarget(RequestForm::ORIGIN, URI("", "", "", "", "/", "", "")),
			"1.1"
		),
		field_lines,
		"HelloWorld!"
	);
	ASSERT_EQ(act, exp);
}
