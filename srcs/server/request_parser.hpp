#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include <deque>

#include "buffer.hpp"
#include "emptiable.hpp"
#include "request.hpp"
#include "request_message.hpp"
#include "result.hpp"
#include "status_code.hpp"

namespace server
{
	class RequestParser
	{

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
		struct Context {
			State                 state;
			std::string           loaded_data;
			http::RequestMessage *request_msg;
		} ctx_;

	  public:
		RequestParser();
		RequestParser(const RequestParser &other);
		~RequestParser();
		RequestParser     &operator=(const RequestParser &rhs);
		Emptiable<Request> Parse(buffer::Buffer &recieved);
		bool               HasInCompleteData();
		void               DestroyParseContext();
		static void        DestroyRequest(Request &request);

	  private:
		void        InitParseContext();
		ParseResult CreateRequestMessage(buffer::Buffer &recieved);
		ParseResult ParseStartLine(buffer::Buffer &recieved);
		ParseResult ParseHeaderSection(buffer::Buffer &recieved);
		ParseResult ParseBody(buffer::Buffer &recieved);
		LoadResult  LoadUntillDelim(buffer::Buffer &recieved, const std::string &delim);
		void        SetStateAndClearBuf(State new_state);
	};
} // namespace server
#endif
