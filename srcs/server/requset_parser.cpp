#include "requset_parser.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "webserv_utils.hpp"

namespace server
{
	RequestParser::RequestParser() : state_(kStandBy), request_ptr_(NULL) {}

	// TODO msg作成部分関数に分ける、msg完成したらqueueにpush_back、kDoneみたいなstateの追加、
	RequestParser::ErrStatus RequestParser::Parse(buffer::Buffer &recieved)
	{
		try {
			CreateRquestMessage(recieved);
			return ErrStatus();
		} catch (http::HttpException &e) {
			utils::DeleteSafe(request_ptr_);
			request_queue_.push_back(Error());
			state_ = kStandBy;
			return ErrStatus(e.GetStatusCode(), Error());
		}
	}

	void RequestParser::CreateRquestMessage(buffer::Buffer &recieved)
	{
		switch (state_) {
		case kStandBy:
			request_ptr_ = new http::RequestMessage();
			state_       = kStartLine;
			/* Falls through. */
		case kStartLine:
			ParseStartLine(recieved);
			break;
		case kHeader:
			ParseHeaderSection(recieved);
			break;
		case kBody:
			ParseBody(recieved);
			break;
		}
	}
	// TODO buffer作成部分関数に分けたいかも
	void RequestParser::ParseStartLine(buffer::Buffer &recieved)
	{
		for (;;) {
			if (recieved.empty()) {
				return;
			}
			Emptiable<char> c = recieved.PopChar();
			buffer_ += c.Value();
			if (utils::EndWith(buffer_, http::kCrLf)) {
				break;
			}
		}
		// 末尾消して、RequsetMessage.SetRequsetLine(http::RequsetLine());みたいにする予定
		// stateの更新
	}

	void RequestParser::ParseHeaderSection(buffer::Buffer &recieved)
	{
		for (;;) {
			if (recieved.empty()) {
				return;
			}
			Emptiable<char> c = recieved.PopChar();
			buffer_ += c.Value();
			if (utils::EndWith(buffer_, http::kEmptyLine)) {
				break;
			}
		}
		// SetHeaderSection()
		// stateの更新
	}

	void RequestParser::ParseBody(buffer::Buffer &recieved)
	{
		(void)recieved;
	}
} // namespace server
