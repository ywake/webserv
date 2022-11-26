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
		ctx_.state        = rhs.ctx_.state;
		ctx_.loaded_data  = rhs.ctx_.loaded_data;
		*ctx_.request_msg = *rhs.ctx_.request_msg;
		return *this;
	}

	RequestParser::~RequestParser()
	{
		delete ctx_.request_msg;
	}

	bool RequestParser::HasInCompleteData()
	{
		return ctx_.state != kStandBy;
	}

	void RequestParser::InitParseContext()
	{
		ctx_.loaded_data = std::string();
		ctx_.state       = kStandBy;
		ctx_.request_msg = new http::RequestMessage();
	}

	Emptiable<Request> RequestParser::Parse(buffer::Buffer &recieved)
	{
		if (recieved.empty()) {
			return Emptiable<Request>();
		}
		try {
			if (CreateRequestMessage(recieved) == kComplete) {
				Request req = ctx_.request_msg;
				InitParseContext();
				return req;
			}
			return Emptiable<Request>();
		} catch (http::HttpException &e) {
			utils::DeleteSafe(ctx_.request_msg);
			InitParseContext();
			return Request(e.GetStatusCode(), Request::kFatal);
		}
	}

	// TODO トレイラ無視してる
	RequestParser::ParseResult RequestParser::CreateRequestMessage(buffer::Buffer &recieved)
	{
		switch (ctx_.state) {
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
		ctx_.loaded_data.erase(ctx_.loaded_data.size() - http::kCrLf.size());
		ctx_.request_msg->SetRequestLine(RequestLine(ctx_.loaded_data));
		SetStateAndClearBuf(kHeader);
		return kInComplete;
	}

	RequestParser::ParseResult RequestParser::ParseHeaderSection(buffer::Buffer &recieved)
	{
		if (LoadUntillDelim(recieved, http::kEmptyLine) != kParsable) {
			return kInComplete;
		}
		ctx_.loaded_data.erase(ctx_.loaded_data.size() - http::kCrLf.size());
		const HeaderSection headers = HeaderSection(ctx_.loaded_data);
		http::headers::ValidateHeaderSection(headers);
		ctx_.request_msg->SetHeaderSection(headers);
		SetStateAndClearBuf(kBody);
		return ctx_.request_msg->HasMessageBody() ? kInComplete : kComplete;
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
			ctx_.loaded_data += c.Value();
			if (utils::EndWith(ctx_.loaded_data, delim)) {
				return kParsable;
			}
		}
	}

	void RequestParser::SetStateAndClearBuf(State new_state)
	{
		ctx_.loaded_data = std::string();
		ctx_.state       = new_state;
	}

	void RequestParser::DestroyRequest(Request &request)
	{
		if (request.IsErr()) {
			return;
		}
		utils::DeleteSafe(request.request_msg_);
	}

} // namespace server
