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
		ctx_.loaded_bytes = rhs.ctx_.loaded_bytes;
		*ctx_.request     = *rhs.ctx_.request;
		return *this;
	}

	RequestParser::~RequestParser()
	{
		delete ctx_.request;
	}

	bool RequestParser::HasInCompleteData()
	{
		return ctx_.state != kStandBy;
	}

	Emptiable<IRequest *> RequestParser::OnEof()
	{
		if (HasInCompleteData()) {
			DestroyParseContext();
			return new Request(http::StatusCode::kBadRequest, Request::kFatal);
		}
		return Emptiable<IRequest *>();
	}

	void RequestParser::InitParseContext()
	{
		ctx_.loaded_bytes = std::string();
		ctx_.state        = kStandBy;
		ctx_.request      = new Request();
	}

	Emptiable<IRequest *> RequestParser::Parse(buffer::QueuingBuffer &recieved)
	{
		if (recieved.empty()) {
			return Emptiable<IRequest *>();
		}
		try {
			if (CreateRequestMessage(recieved) == kComplete) {
				Request *req = ctx_.request;
				InitParseContext();
				return req;
			}
			return Emptiable<IRequest *>();
		} catch (http::HttpException &e) {
			DestroyParseContext();
			return new Request(e.GetStatusCode(), Request::kFatal);
		}
	}

	// TODO トレイラ無視してる
	RequestParser::ParseResult RequestParser::CreateRequestMessage(buffer::QueuingBuffer &recieved)
	{
		switch (ctx_.state) {
		case kStandBy:
			SetStateAndClearLoadedBytes(kStartLine);
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

	RequestParser::ParseResult RequestParser::ParseStartLine(buffer::QueuingBuffer &recieved)
	{
		if (LoadUntillDelim(recieved, http::kCrLf) != kParsable) {
			return kInComplete;
		}
		ctx_.loaded_bytes.erase(ctx_.loaded_bytes.size() - http::kCrLf.size());
		// ctx_.request->SetRequestLine(RequestLine(ctx_.loaded_bytes));
		SetStateAndClearLoadedBytes(kHeader);
		return kInComplete;
	}

	RequestParser::ParseResult RequestParser::ParseHeaderSection(buffer::QueuingBuffer &recieved)
	{
		if (LoadUntillDelim(recieved, http::kEmptyLine) != kParsable) {
			return kInComplete;
		}
		ctx_.loaded_bytes.erase(ctx_.loaded_bytes.size() - http::kCrLf.size());
		const HeaderSection headers = HeaderSection(ctx_.loaded_bytes);
		http::headers::ValidateHeaderSection(headers);
		ctx_.request->SetHeaderSection(headers);
		SetStateAndClearLoadedBytes(kBody);
		return ctx_.request->HasMessageBody() ? kInComplete : kComplete;
	}

	// TODO body
	RequestParser::ParseResult RequestParser::ParseBody(buffer::QueuingBuffer &recieved)
	{
		(void)recieved;
		return kComplete;
	}

	RequestParser::LoadResult
	RequestParser::LoadUntillDelim(buffer::QueuingBuffer &recieved, const std::string &delim)
	{
		for (;;) {
			if (recieved.empty()) {
				return kNonParsable;
			}
			Emptiable<char> c = recieved.PopChar();
			ctx_.loaded_bytes += c.Value();
			if (utils::EndWith(ctx_.loaded_bytes, delim)) {
				return kParsable;
			}
		}
	}

	void RequestParser::SetStateAndClearLoadedBytes(State new_state)
	{
		ctx_.loaded_bytes = std::string();
		ctx_.state        = new_state;
	}

	void RequestParser::DestroyParseContext()
	{
		utils::DeleteSafe(ctx_.request);
		InitParseContext();
	}

	void RequestParser::DestroyRequest(IRequest *&request)
	{
		if (request->IsErr()) {
			return;
		}
		utils::DeleteSafe(request);
	}

	IRequest *RequestParser::CopyRequest(const IRequest *src)
	{
		return new Request(src->GetMessage(), src->GetErrStatusCode(), src->GetErrorType());
	}

} // namespace server
