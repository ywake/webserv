#include "requset_holder.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "validate_headers.hpp"
#include "webserv_utils.hpp"

namespace server
{
	RequestHolder::RequestHolder() : request_queue_()
	{
		InitParseContext();
	}

	RequestHolder::~RequestHolder()
	{
		delete request_ptr_;
		while (!request_queue_.empty()) {
			DeleteFront();
		}
	}

	Result<void> RequestHolder::DeleteFront()
	{
		if (request_queue_.empty()) {
			return Error("DeleteRequest: empty");
		}
		delete (request_queue_.front().Val());
		request_queue_.pop_front();
		return Result<void>();
	}

	std::size_t RequestHolder::Count()
	{
		return request_queue_.size();
	}

	bool RequestHolder::HasInCompleteData()
	{
		return state_ != kStandBy;
	}

	void RequestHolder::InitParseContext()
	{
		buffer_      = std::string();
		state_       = kStandBy;
		request_ptr_ = new http::RequestMessage();
	}

	RequestHolder::ErrStatus RequestHolder::Parse(buffer::Buffer &recieved)
	{
		if (recieved.empty()) {
			return ErrStatus();
		}
		try {
			if (CreateRquestMessage(recieved) == kComplete) {
				request_queue_.push_back(request_ptr_);
				request_ptr_ = NULL;
				InitParseContext();
			}
			return ErrStatus();
		} catch (http::HttpException &e) {
			request_queue_.push_back(Error());
			utils::DeleteSafe(request_ptr_);
			InitParseContext();
			return ErrStatus(e.GetStatusCode(), Error());
		}
	}

	// TODO トレイラ無視してる
	RequestHolder::ParseResult RequestHolder::CreateRquestMessage(buffer::Buffer &recieved)
	{
		switch (state_) {
		case kStandBy:
			SetStateAndClearBuf(kStartLine);
			/* Falls through. */
		case kStartLine:
			return ParseStartLine(recieved);
		case kHeader:
			return ParseHeaderSection(recieved);
		case kBody:
			return ParseBody(recieved);
		}
		return kInComplete;
	}

	RequestHolder::ParseResult RequestHolder::ParseStartLine(buffer::Buffer &recieved)
	{
		if (LoadUntillDelim(recieved, http::kCrLf) != kParsable) {
			return kInComplete;
		}
		buffer_.erase(buffer_.size() - http::kCrLf.size());
		request_ptr_->SetRequestLine(RequestLine(buffer_));
		SetStateAndClearBuf(kHeader);
		return kInComplete;
	}

	RequestHolder::ParseResult RequestHolder::ParseHeaderSection(buffer::Buffer &recieved)
	{
		if (LoadUntillDelim(recieved, http::kEmptyLine) != kParsable) {
			return kInComplete;
		}
		buffer_.erase(buffer_.size() - http::kCrLf.size());
		const HeaderSection headers = HeaderSection(buffer_);
		http::headers::ValidateHeaderSection(headers);
		request_ptr_->SetHeaderSection(headers);
		SetStateAndClearBuf(kBody);
		return request_ptr_->HasMessageBody() ? kInComplete : kComplete;
	}

	// TODO body
	RequestHolder::ParseResult RequestHolder::ParseBody(buffer::Buffer &recieved)
	{
		(void)recieved;
		return kComplete;
	}

	RequestHolder::LoadResult
	RequestHolder::LoadUntillDelim(buffer::Buffer &recieved, const std::string &delim)
	{
		for (;;) {
			if (recieved.empty()) {
				return kNonParsable;
			}
			Emptiable<char> c = recieved.PopChar();
			buffer_ += c.Value();
			if (utils::EndWith(buffer_, delim)) {
				return kParsable;
			}
		}
	}

	void RequestHolder::SetStateAndClearBuf(State new_state)
	{
		buffer_ = std::string();
		state_  = new_state;
	}

} // namespace server
