#include <cassert>

#include "host_port.hpp"
#include "http_define.hpp"
#include "request_parser.hpp"

namespace server
{
	RequestParser::Request::Request()
		: request_line_(), field_section_(), body_(), error_code_(), error_type_()
	{}

	RequestParser::Request::Request(const Request &other)
		: request_line_(other.request_line_),
		  field_section_(),
		  body_(),
		  error_code_(other.error_code_),
		  error_type_(other.error_type_)
	{}

	RequestParser::Request::Request(const http::StatusCode &error_code, ErrorType error_type)
		: request_line_(),
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
		return field_section_->Contains(http::kContentLength) ||
			   field_section_->Contains(http::kTransferEncoding);
	}

	void RequestParser::Request::SetError(const http::StatusCode &error_code, ErrorType error_type)
	{
		error_code_ = error_code;
		error_type_ = error_type;
	}

	void RequestParser::Request::SetRequestLine(const RequestLine &request_line)
	{
		request_line_ = request_line;
	}

	void RequestParser::Request::SetFieldSection(http::FieldSection *field_section)
	{
		field_section_ = field_section;
	}

	void RequestParser::Request::SetBody(const std::vector<char> *body)
	{
		body_ = body;
	}

	// host port以外はサボり
	void RequestParser::Request::ReconstructUri()
	{
		RequestTarget::FormType from_type = request_line_.GetRequestTarget().GetFormType();
		if (from_type == RequestTarget::kAbsoluteForm || field_section_ == NULL) {
			return;
		}
		http::abnf::HostPort hostport =
			http::abnf::HostPort((*field_section_)["host"].front().GetValue());
		request_line_.SetHost(hostport.GetHost().ToString());
		request_line_.SetPort(hostport.GetPort().ToString());
	}

	const std::string &RequestParser::Request::Method() const
	{
		return request_line_.GetMethod();
	}

	const std::string &RequestParser::Request::Path() const
	{
		return request_line_.GetRequestTarget().GetRequestFormData().path_;
	}

	const std::string &RequestParser::Request::Query() const
	{
		return request_line_.GetRequestTarget().GetRequestFormData().query_;
	}

	const std::string &RequestParser::Request::Host() const
	{
		return request_line_.GetRequestTarget().GetRequestFormData().host_;
	}

	const std::string &RequestParser::Request::Port() const
	{
		return request_line_.GetRequestTarget().GetRequestFormData().port_;
	}

	std::string RequestParser::Request::Authority() const
	{
		std::string authority;

		const std::string &user_info =
			request_line_.GetRequestTarget().GetRequestFormData().userinfo_;
		if (!user_info.empty()) {
			authority += user_info + "@";
		}
		authority += Host();
		if (!Port().empty()) {
			authority += ":" + Port();
		}
		return authority;
	}

	const http::FieldSection &RequestParser::Request::Headers() const
	{
		return *field_section_;
	}

	http::FieldSection &RequestParser::Request::Headers()
	{
		return *field_section_;
	}

	const RequestLine &RequestParser::Request::GetRequestLine() const
	{
		return request_line_;
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

	void RequestParser::Request::SetPath(const std::string &path)
	{
		request_line_.SetPath(path);
	}

	void RequestParser::Request::SetQuery(const std::string &query)
	{
		request_line_.SetQuery(query);
	}

	bool RequestParser::Request::NeedToClose() const
	{
		if (error_type_ == IRequest::kFatal) {
			return true;
		}
		if (field_section_ == NULL) {
			return false;
		}
		http::FieldSection::Values con = (*field_section_)[http::kConnection];
		for (http::FieldSection::Values::const_iterator it = con.begin(); it != con.end(); it++) {
			if (it->GetValue() == "close") {
				return true;
			}
		}
		return false;
	}

} // namespace server
