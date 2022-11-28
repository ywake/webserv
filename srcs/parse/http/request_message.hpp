#ifndef REQUEST_MESSAGE_HPP
#define REQUEST_MESSAGE_HPP

#include "header_section.hpp"
#include "http_message.hpp"
#include "request_target.hpp"

namespace http
{
	class RequestMessage : public HttpMessage
	{
	  private:
		std::string   method_;
		RequestTarget request_target_;
		std::string   http_version_;

	  public:
		RequestMessage();
		RequestMessage(const RequestMessage &other);
		RequestMessage(
			const std::string   &method,
			const RequestTarget &request_target,
			const std::string   &http_version,
			const HeaderSection &field_lines,
			const std::string   &message_body
		);
		~RequestMessage();
		void            SetMethod(const std::string &method);
		void            SetRequestTarget(const RequestTarget &request_target);
		void            SetHttpVersion(const std::string &http_version);
		RequestMessage &operator=(const RequestMessage &rhs);
		bool            operator==(const RequestMessage &rhs) const;
	};
} // namespace http
#endif
