#include "requset_parser.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "validate_headers.hpp"
#include "webserv_utils.hpp"

namespace server
{
	RequestParser::RequestParser() : state_(kStandBy), request_ptr_(NULL) {}

	RequestParser::~RequestParser()
	{
		delete request_ptr_;
		while (!request_queue_.empty()) {
			DeleteRequest();
		}
	}

	Result<void> RequestParser::DeleteRequest()
	{
		if (request_queue_.empty()) {
			return Error("DeleteRequest: empty");
		}
		delete (request_queue_.front().Val());
		request_queue_.pop_front();
		return Result<void>();
	}

	RequestParser::ErrStatus RequestParser::Parse(buffer::Buffer &recieved)
	{
		try {
			if (CreateRquestMessage(recieved) == kComplete) {
				request_queue_.push_back(request_ptr_);
				request_ptr_ = NULL;
				state_       = kStandBy;
			}
			return ErrStatus();
		} catch (http::HttpException &e) {
			utils::DeleteSafe(request_ptr_);
			request_queue_.push_back(Error());
			state_ = kStandBy;
			return ErrStatus(e.GetStatusCode(), Error());
		}
	}

	// TODO トレイラ無視してる
	RequestParser::ParseResult RequestParser::CreateRquestMessage(buffer::Buffer &recieved)
	{
		switch (state_) {
		case kStandBy:
			request_ptr_ = new http::RequestMessage();
			state_       = kStartLine;
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

	RequestParser::ParseResult RequestParser::ParseStartLine(buffer::Buffer &recieved)
	{
		if (LoadUntillDelim(recieved, http::kCrLf) != kParsable) {
			return kInComplete;
		}
		buffer_.erase(buffer_.size() - http::kCrLf.size());
		request_ptr_->SetRequestLine(RequestLine(buffer_));
		AdvanceState();
		return kInComplete;
	}

	RequestParser::ParseResult RequestParser::ParseHeaderSection(buffer::Buffer &recieved)
	{
		if (LoadUntillDelim(recieved, http::kEmptyLine) != kParsable) {
			return kInComplete;
		}
		buffer_.erase(buffer_.size() - http::kCrLf.size());
		const HeaderSection headers = HeaderSection(buffer_);
		http::headers::ValidateHeaderSection(headers);
		request_ptr_->SetHeaderSection(headers);
		AdvanceState();
		return request_ptr_->HasMessageBody() ? kInComplete : kComplete;
	}

	RequestParser::ParseResult RequestParser::ParseBody(buffer::Buffer &recieved)
	{
		(void)recieved;
		return kComplete;
	}

	RequestParser::LoadResult
	RequestParser::LoadUntillDelim(buffer::Buffer &recieved, const std::string &delim)
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

	// トレイラ
	void RequestParser::AdvanceState()
	{
		buffer_ = std::string();
		switch (state_) {
		case kStandBy:
			break;
		case kStartLine:
			state_ = kHeader;
			break;
		case kHeader:
			state_ = kBody;
			break;
		case kBody:
			state_ = kStandBy;
			break;
		}
	}

} // namespace server
