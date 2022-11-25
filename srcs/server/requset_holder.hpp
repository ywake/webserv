#ifndef REQUSET_HOLDER_HPP
#define REQUSET_HOLDER_HPP

#include <deque>

#include "buffer.hpp"
#include "request.hpp"
#include "result.hpp"
#include "status_code.hpp"

namespace server
{
	class RequestHolder
	{
	  private:
		typedef std::deque<Request> RequestQueue;

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
		std::string           loaded_data_;
		State                 state_;
		http::RequestMessage *request_ptr_;
		RequestQueue          request_queue_;

	  public:
		RequestHolder();
		RequestHolder(const RequestHolder &other);
		~RequestHolder();
		RequestHolder &operator=(const RequestHolder &rhs);
		void           Parse(buffer::Buffer &recieved);
		void           DeleteFront();
		void           DeleteAll();
		std::size_t    Count();
		bool           HasInCompleteData();

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
