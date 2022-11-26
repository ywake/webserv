#ifndef REQUEST_HOLDER_HPP
#define REQUEST_HOLDER_HPP

#include <deque>

#include "buffer.hpp"
#include "emptiable.hpp"
#include "request.hpp"
#include "request_parser.hpp"
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
		Emptiable<Request> in_progress;
		RequestQueue       request_queue_;
		RequestParser      parser_;

	  public:
		RequestHolder();
		RequestHolder(const RequestHolder &other);
		~RequestHolder();

		RequestHolder &operator=(const RequestHolder &rhs);

		void        Parse(buffer::Buffer &recieved);
		std::size_t Count();
		void        OnEof();

	  private:
		void DestroyQueue();
	};
} // namespace server
#endif
