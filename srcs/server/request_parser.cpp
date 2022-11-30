#include "request_parser.hpp"
#include "debug.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "method_pool.hpp"
#include "validate_headers.hpp"
#include "validate_request_line.hpp"
#include "webserv_utils.hpp"

namespace server
{
	const std::size_t RequestParser::kMaxHeaderSectonSize = 8196 * 4;

	RequestParser::RequestParser()
	{
		InitParseContext();
	}

	RequestParser::RequestParser(const RequestParser &other) : StatefulParser()
	{
		*this = other;
	}

	RequestParser &RequestParser::operator=(const RequestParser &rhs)
	{
		if (this == &rhs) {
			return *this;
		}
		StatefulParser::operator=(rhs);
		ctx_.request_line_parser = rhs.ctx_.request_line_parser;
		ctx_.state               = rhs.ctx_.state;
		*ctx_.request            = *rhs.ctx_.request;
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
		ClearLoadedBytes();
		ctx_.request_line_parser = RequestLineParser();
		ctx_.state               = kStandBy;
		ctx_.request             = new Request();
	}

	Emptiable<IRequest *> RequestParser::Parse(q_buffer::QueuingBuffer &recieved)
	{
		if (recieved.empty()) {
			return Emptiable<IRequest *>();
		}
		try {
			if (CreateRequestMessage(recieved) == kDone) {
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

	RequestParser::ParseResult RequestParser::CreateRequestMessage(q_buffer::QueuingBuffer &recieved
	)
	{
		while (!recieved.empty()) {
			switch (ParseEachPhase(recieved)) {
			case kInProgress:
				return kInProgress;
			case kDone:
				if (ctx_.state == kBody ||
					(ctx_.state == kHeader && !ctx_.request->HasMessageBody())) {
					return kDone;
				}
				ctx_.state = GetNextState(ctx_.state);
				ClearLoadedBytes();
			}
		}
		return kInProgress;
	}

	// TODO トレイラ無視してる
	RequestParser::ParseResult RequestParser::ParseEachPhase(q_buffer::QueuingBuffer &recieved)
	{
		switch (ctx_.state) {
		case kStandBy:
			return kInProgress;
		case kStartLine:
			return ParseStartLine(recieved);
		case kHeader:
			return ParseHeaderSection(recieved);
		case kBody:
			return ParseBody(recieved);
		}
		return kInProgress;
	}

	RequestParser::ParseResult RequestParser::ParseStartLine(q_buffer::QueuingBuffer &recieved)
	{
		Emptiable<RequestLine> req_line = ctx_.request_line_parser.Parse(recieved);
		if (req_line.empty()) {
			return kInProgress;
		}
		ctx_.request->SetRequestLine(req_line.Value());
		return kDone;
	}

	RequestParser::ParseResult RequestParser::ParseHeaderSection(q_buffer::QueuingBuffer &recieved)
	{
		switch (LoadBytesWithDelim(recieved, http::kEmptyLine, kMaxHeaderSectonSize)) {
		case kOverMaxSize:
			throw http::BadRequestException();
		case kParsable: {
			loaded_bytes_.erase(loaded_bytes_.size() - http::kCrLf.size());
			const HeaderSection headers = HeaderSection(loaded_bytes_);
			http::headers::ValidateHeaderSection(headers);
			ctx_.request->SetHeaderSection(headers);
			return kDone;
		}
		case kNonParsable:
			return kInProgress;
		}
		return kInProgress;
	}

	// TODO body
	RequestParser::ParseResult RequestParser::ParseBody(q_buffer::QueuingBuffer &recieved)
	{
		(void)recieved;
		return kDone;
	}

	RequestParser::State RequestParser::GetNextState(State old_state)
	{
		switch (old_state) {
		case kStandBy:
			return kStartLine;
		case kStartLine:
			return kHeader;
		case kHeader:
			return kBody;
		case kBody:
			return kStandBy;
		}
		return kStandBy;
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
