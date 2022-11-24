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
		typedef std::deque<Result<http::RequestMessage *> > RequestQueue;
		typedef Result<http::StatusCode>                    ErrStatus;

	  private:
		enum State {
			kStandBy,
			kStartLine,
			kHeader,
			kBody
		};

	  private:
		std::string           buffer_;
		State                 state_;
		http::RequestMessage *request_ptr_;
		RequestQueue          request_queue_;

	  public:
		RequestParser();
		ErrStatus Parse(buffer::Buffer &recieved);

	  private:
		void CreateRquestMessage(buffer::Buffer &recieved);
		void ParseStartLine(buffer::Buffer &recieved);
		void ParseHeaderSection(buffer::Buffer &recieved);
		void ParseBody(buffer::Buffer &recieved);
	};
} // namespace server
#endif
