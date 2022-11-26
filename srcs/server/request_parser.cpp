#include "request_parser.hpp"
#include "debug.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "validate_headers.hpp"
#include "webserv_utils.hpp"

namespace server
{
	RequestParser::RequestParser()
	{
		InitParseContext();
	}

	RequestParser::RequestParser(const RequestParser &other)
	{
		*this = other;
	}

	RequestParser &RequestParser::operator=(const RequestParser &rhs)
	{
		if (this == &rhs) {
			return *this;
		}
		state_        = rhs.state_;
		loaded_data_  = rhs.loaded_data_;
		*request_ptr_ = *rhs.request_ptr_;
		return *this;
	}

	RequestParser::~RequestParser()
	{
		delete request_ptr_;
	}

	bool RequestParser::HasInCompleteData()
	{
		return state_ != kStandBy;
	}

	void RequestParser::InitParseContext()
	{
		loaded_data_ = std::string();
		state_       = kStandBy;
		request_ptr_ = new http::RequestMessage();
	}

	Emptiable<Request> RequestParser::Parse(buffer::Buffer &recieved)
	{
		if (recieved.empty()) {
			return Emptiable<Request>();
		}
		try {
			if (CreateRequestMessage(recieved) == kComplete) {
				Request req = request_ptr_;
				InitParseContext();
				return req;
			}
			return Emptiable<Request>();
		} catch (http::HttpException &e) {
			utils::DeleteSafe(request_ptr_);
			InitParseContext();
			return Request(e.GetStatusCode(), Request::kFatal);
		}
	}

	// TODO トレイラ無視してる
	RequestParser::ParseResult RequestParser::CreateRequestMessage(buffer::Buffer &recieved)
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

	RequestParser::ParseResult RequestParser::ParseStartLine(buffer::Buffer &recieved)
	{
		if (LoadUntillDelim(recieved, http::kCrLf) != kParsable) {
			return kInComplete;
		}
		loaded_data_.erase(loaded_data_.size() - http::kCrLf.size());
		request_ptr_->SetRequestLine(RequestLine(loaded_data_));
		SetStateAndClearBuf(kHeader);
		return kInComplete;
	}

	RequestParser::ParseResult RequestParser::ParseHeaderSection(buffer::Buffer &recieved)
	{
		if (LoadUntillDelim(recieved, http::kEmptyLine) != kParsable) {
			return kInComplete;
		}
		loaded_data_.erase(loaded_data_.size() - http::kCrLf.size());
		const HeaderSection headers = HeaderSection(loaded_data_);
		http::headers::ValidateHeaderSection(headers);
		request_ptr_->SetHeaderSection(headers);
		SetStateAndClearBuf(kBody);
		return request_ptr_->HasMessageBody() ? kInComplete : kComplete;
	}

	// TODO body
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
			loaded_data_ += c.Value();
			if (utils::EndWith(loaded_data_, delim)) {
				return kParsable;
			}
		}
	}

	void RequestParser::SetStateAndClearBuf(State new_state)
	{
		loaded_data_ = std::string();
		state_       = new_state;
	}

	void RequestParser::DestroyRequest(Request &request)
	{
		if (request.IsErr()) {
			return;
		}
		utils::DeleteSafe(request.request_msg_);
	}

} // namespace server
