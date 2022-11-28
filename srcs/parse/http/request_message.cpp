#include "request_message.hpp"

namespace http
{
	RequestMessage::RequestMessage() : HttpMessage(), method_(), request_target_(), http_version_()
	{}

	RequestMessage::RequestMessage(const RequestMessage &other) : HttpMessage(other)
	{
		method_         = other.method_;
		request_target_ = other.request_target_;
		http_version_   = other.http_version_;
	}

	RequestMessage::RequestMessage(
		const std::string   &method,
		const RequestTarget &request_target,
		const std::string   &http_version,
		const HeaderSection &field_lines,
		const std::string   &message_body
	)
		: HttpMessage(field_lines, message_body),
		  method_(method),
		  request_target_(request_target),
		  http_version_(http_version)
	{}

	void RequestMessage::SetMethod(const std::string &method)
	{
		method_ = method;
	}

	void RequestMessage::SetRequestTarget(const RequestTarget &request_target)
	{
		request_target_ = request_target;
	}

	void RequestMessage::SetHttpVersion(const std::string &http_version)
	{
		http_version_ = http_version;
	}

	RequestMessage &RequestMessage::operator=(const RequestMessage &rhs)
	{
		if (this == &rhs) {
			return *this;
		}
		HttpMessage::operator=(rhs);
		method_         = rhs.method_;
		request_target_ = rhs.request_target_;
		http_version_   = rhs.http_version_;
		return *this;
	}

	bool RequestMessage::operator==(const RequestMessage &rhs) const
	{
		return HttpMessage::operator==(rhs) && method_ == rhs.method_ &&
			   request_target_ == rhs.request_target_ && http_version_ == rhs.http_version_;
	}

	RequestMessage::~RequestMessage() {}

} // namespace http
