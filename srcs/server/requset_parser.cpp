#include "requset_parser.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "webserv_utils.hpp"

namespace server
{
	RequestParser::RequestParser() : state_(kStandBy), request_ptr_(NULL) {}

	Result<void> RequestParser::DeleteRequest()
	{
		if (request_queue_.empty()) {
			return Error("DeleteRequest: empty");
		}
		delete (request_queue_.front().Val());
		request_queue_.pop_front();
		return Result<void>();
	}

	// TODO msg作成部分関数に分ける、msg完成したらqueueにpush_back、kDoneみたいなstateの追加、
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
	// TODO buffer作成部分関数に分けたいかも
	RequestParser::ParseResult RequestParser::ParseStartLine(buffer::Buffer &recieved)
	{
		for (;;) {
			if (recieved.empty()) {
				return kInComplete;
			}
			Emptiable<char> c = recieved.PopChar();
			buffer_ += c.Value();
			if (utils::EndWith(buffer_, http::kCrLf)) {
				break;
			}
		}
		// 末尾消して、RequsetMessage.SetRequsetLine(http::RequsetLine());みたいにする予定
		// stateの更新
		return kInComplete;
	}

	RequestParser::ParseResult RequestParser::ParseHeaderSection(buffer::Buffer &recieved)
	{
		for (;;) {
			if (recieved.empty()) {
				return kInComplete;
			}
			Emptiable<char> c = recieved.PopChar();
			buffer_ += c.Value();
			if (utils::EndWith(buffer_, http::kEmptyLine)) {
				break;
			}
		}
		// SetHeaderSection()
		// stateの更新
		return request_ptr_->HasMessageBody() ? kInComplete : kComplete;
	}

	RequestParser::ParseResult RequestParser::ParseBody(buffer::Buffer &recieved)
	{
		(void)recieved;
		return kComplete;
	}
} // namespace server
