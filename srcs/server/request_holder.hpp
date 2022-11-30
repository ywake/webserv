#ifndef REQUEST_HOLDER_HPP
#define REQUEST_HOLDER_HPP

#include <deque>

#include "emptiable.hpp"
#include "queuing_buffer.hpp"
#include "request_parser.hpp"
#include "result.hpp"
#include "status_code.hpp"

namespace server
{
	class RequestHolder
	{
	  private:
		typedef std::deque<IRequest *> RequestQueue;

	  private:
		RequestQueue  request_queue_;
		RequestParser parser_;

	  public:
		RequestHolder();
		RequestHolder(const RequestHolder &other);
		~RequestHolder();

		RequestHolder &operator=(const RequestHolder &rhs);

		void                  Parse(q_buffer::QueuingBuffer &recieved);
		Emptiable<IRequest *> PopFront();
		std::size_t           size();
		void                  OnEof();

		static void DestroyRequest(IRequest *&request);

	  private:
		void DestroyQueue();
	};
} // namespace server
#endif
