#include "request_holder.hpp"
#include "debug.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "validate_headers.hpp"
#include "webserv_utils.hpp"

namespace server
{
	RequestHolder::RequestHolder() : request_queue_() {}

	RequestHolder::RequestHolder(const RequestHolder &other)
	{
		*this = other;
	}

	RequestHolder::~RequestHolder()
	{
		if (!in_progress.empty()) {
			RequestParser::DestroyRequest(in_progress.Value());
		}
		DestroyQueue();
	}

	void RequestHolder::Parse(buffer::Buffer &recieved)
	{
		Emptiable<IRequest *> req = parser_.Parse(recieved);
		if (req.empty()) {
			return;
		}
		request_queue_.push_back(req.Value());
	}

	void RequestHolder::DestroyQueue()
	{
		while (!request_queue_.empty()) {
			RequestParser::DestroyRequest(request_queue_.front());
			request_queue_.pop_front();
		}
	}

	std::size_t RequestHolder::Count()
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
			request_queue_.push_back(RequestParser::CopyRequest(*it));
		}
		return *this;
	}

} // namespace server
