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
		enum ParseResult {
			kComplete,
			kInComplete
		};
		enum LoadResult {
			kParsable,
			kNonParsable
		};

	  private:
		std::string           buffer_;
		State                 state_;
		http::RequestMessage *request_ptr_;
		RequestQueue          request_queue_;

	  public:
		RequestParser();
		~RequestParser();
		ErrStatus    Parse(buffer::Buffer &recieved);
		Result<void> DeleteRequest();
		std::size_t  Count();

	  private:
		ParseResult CreateRquestMessage(buffer::Buffer &recieved);
		ParseResult ParseStartLine(buffer::Buffer &recieved);
		ParseResult ParseHeaderSection(buffer::Buffer &recieved);
		ParseResult ParseBody(buffer::Buffer &recieved);
		LoadResult  LoadUntillDelim(buffer::Buffer &recieved, const std::string &delim);
		void        AdvanceState();
	};
} // namespace server
#endif
