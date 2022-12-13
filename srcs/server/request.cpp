#include <cassert>

#include "request_parser.hpp"
namespace server
{
	RequestParser::Request::Request()
		: request_msg_(), field_section_(), body_(), error_code_(), error_type_()
	{}

	RequestParser::Request::Request(const Request &other)
		: request_msg_(other.request_msg_),
		  field_section_(),
		  body_(),
		  error_code_(other.error_code_),
		  error_type_(other.error_type_)
	{}

	RequestParser::Request::Request(const http::StatusCode &error_code, ErrorType error_type)
		: request_msg_(),
		  field_section_(),
		  body_(),
		  error_code_(error_code),
		  error_type_(error_type)
	{}

	RequestParser::Request::~Request() {}

	bool RequestParser::Request::IsErr() const
	{
		return error_type_ != kNotError;
	}

	bool RequestParser::Request::IsFatal() const
	{
		return error_type_ == kFatal;
	}

	bool RequestParser::Request::HasMessageBody() const
	{
		return request_msg_.HasMessageBody();
	}

	void RequestParser::Request::SetError(const http::StatusCode &error_code, ErrorType error_type)
	{
		error_code_ = error_code;
		error_type_ = error_type;
	}

	void RequestParser::Request::SetRequestLine(const RequestLine &request_line)
	{
		request_msg_.SetRequestLine(request_line);
	}

	void RequestParser::Request::SetFieldSection(const http::FieldSection *field_section)
	{
		field_section_ = field_section;
	}

	void RequestParser::Request::SetBody(const std::vector<char> *body)
	{
		body_ = body;
	}

	const std::string &RequestParser::Request::Method() const
	{
		return request_msg_.GetRequestLine().GetMethod();
	}

	const std::string &RequestParser::Request::Path() const
	{
		return request_msg_.GetRequestLine().GetRequestTarget().GetRequestFormData().path_;
	}

	const http::FieldSection &RequestParser::Request::Headers() const
	{
		return *field_section_;
	}

	const http::RequestMessage &RequestParser::Request::GetMessage() const
	{
		return request_msg_;
	}

	const http::StatusCode &RequestParser::Request::GetErrStatusCode() const
	{
		return error_code_;
	}

	const RequestParser::Request::ErrorType &RequestParser::Request::GetErrorType() const
	{
		return error_type_;
	}

	const std::vector<char> *RequestParser::Request::GetBody() const
	{
		return body_;
	}

} // namespace server
