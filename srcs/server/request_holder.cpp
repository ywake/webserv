#include "request_holder.hpp"
#include "debug.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "validate_headers.hpp"
#include "webserv_utils.hpp"

namespace server
{
	RequestHolder::RequestHolder(const conf::VirtualServerConfs *config)
		: config_(config), request_queue_(), parser_(config)
	{}

	RequestHolder::RequestHolder(const RequestHolder &other)
	{
		*this = other;
	}

	RequestHolder::~RequestHolder()
	{
		DestroyQueue();
	}

	void RequestHolder::Parse(q_buffer::QueuingBuffer &received)
	{
		Emptiable<IRequest *> req = parser_.Parse(received);
		if (req.empty()) {
			return;
		}
		request_queue_.push_back(req.Value());
	}

	Emptiable<IRequest *> RequestHolder::PopFront()
	{
		if (request_queue_.empty()) {
			return Emptiable<IRequest *>();
		}
		Emptiable<IRequest *> req = request_queue_.front();
		request_queue_.pop_front();
		return req;
	}

	void RequestHolder::DestroyQueue()
	{
		while (!request_queue_.empty()) {
			RequestParser::DestroyIRequest(request_queue_.front());
			request_queue_.pop_front();
		}
	}

	std::size_t RequestHolder::size()
	{
		return request_queue_.size();
	}

	void RequestHolder::OnEof()
	{
		Emptiable<IRequest *> error_req = parser_.OnEof();
		if (error_req.empty()) {
			return;
		}
		request_queue_.push_back(error_req.Value());
	}

	RequestHolder &RequestHolder::operator=(const RequestHolder &rhs)
	{
		if (this == &rhs) {
			return *this;
		}
		DestroyQueue();
		for (RequestQueue::const_iterator it = rhs.request_queue_.begin();
			 it != rhs.request_queue_.end();
			 it++) {
			request_queue_.push_back(RequestParser::CopyIRequest(*it));
		}
		config_ = rhs.config_;
		parser_ = rhs.parser_;
		return *this;
	}

	void RequestHolder::DestroyRequest(IRequest *&request)
	{
		RequestParser::DestroyIRequest(request);
	}
} // namespace server
