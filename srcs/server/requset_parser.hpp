#ifndef REQUSET_PARSER_HPP
#define REQUSET_PARSER_HPP

#include <deque>

#include "buffer.hpp"
#include "request_message.hpp"
#include "result.hpp"
#include "status_code.hpp"

namespace server
{
	class RequestParser
	{
	  private:
		typedef std::deque<Result<HttpMessage> > RequestQueue;
		typedef Result<http::StatusCode>         ErrStatus;

	  private:
		enum State {
			kStandBy,
			kStartLine,
			kHeader,
			kBody
		};

	  private:
		std::string buffer_;
		State       state_;

	  public:
		RequestParser();
		ErrStatus Parse(buffer::Buffer &recieved);

	  private:
		ErrStatus ParseStartLine(buffer::Buffer &recieved);
		ErrStatus ParseHeaderSection(buffer::Buffer &recieved);
		ErrStatus ParseBody(buffer::Buffer &recieved);
	};
} // namespace server
#endif
