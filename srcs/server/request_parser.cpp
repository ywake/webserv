#include "request_parser.hpp"
#include "debug.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "implemented_methods.hpp"
#include "validate_headers.hpp"
#include "validate_request_line.hpp"
#include "webserv_utils.hpp"

namespace server
{
	const std::size_t RequestParser::kMaxHeaderSectonSize = 8196 * 4;

	RequestParser::RequestParser(const conf::VirtualServerConfs *config) : config_(config)
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
		config_                  = rhs.config_;
		ctx_.request_line_parser = rhs.ctx_.request_line_parser;
		ctx_.header_parser       = rhs.ctx_.header_parser;
		ctx_.body_parser         = rhs.ctx_.body_parser;
		ctx_.state               = rhs.ctx_.state;
		IRequest *req            = ctx_.request;
		DestroyRequest(req);
		ctx_.request = CopyRequest(rhs.ctx_.request);
		return *this;
	}

	RequestParser::~RequestParser()
	{
		delete ctx_.request;
	}

	bool RequestParser::HasInCompleteData()
	{
		return ctx_.state != kStandBy || !loaded_bytes_.empty();
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
		ctx_.header_parser       = HeaderParser();
		ctx_.body_parser         = BodyParser(config_);
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
			log("request parser", e.what());
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
				log("next parse state");
			}
		}
		return kInProgress;
	}

	RequestParser::ParseResult RequestParser::SkipPriorCrLf(q_buffer::QueuingBuffer &recieved)
	{
		for (;;) {
			if (recieved.empty()) {
				return kInProgress;
			}
			Emptiable<char> c = recieved.PopChar();
			loaded_bytes_ += c.Value();
			if (utils::EndWith(loaded_bytes_, http::kCrLf)) {
				ClearLoadedBytes();
			}
			if (loaded_bytes_.size() >= http::kCrLf.size()) {
				recieved.push_front(loaded_bytes_);
				ClearLoadedBytes();
				return kDone;
			}
		}
	}

	RequestParser::ParseResult RequestParser::ParseEachPhase(q_buffer::QueuingBuffer &recieved)
	{
		switch (ctx_.state) {
		case kStandBy:
			return SkipPriorCrLf(recieved);
		case kStartLine:
			log("start line");
			return ParseStartLine(recieved);
		case kHeader: {
			log("header");
			ParseResult res = ParseHeaderSection(recieved);
			if (!(res == kDone && ctx_.request->HasMessageBody())) {
				return res;
			}
			ctx_.state = kBody;
		}
		/* Falls through. */
		case kBody:
			log("body");
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
		Emptiable<http::FieldSection *> headers = ctx_.header_parser.Parse(recieved);
		if (headers.empty()) {
			return kInProgress;
		}
		ctx_.request->SetFieldSection(headers.Value());
		return kDone;
	}

	// TODO body
	RequestParser::ParseResult RequestParser::ParseBody(q_buffer::QueuingBuffer &recieved)
	{
		Emptiable<std::vector<char> *> body =
			ctx_.body_parser.Parse(recieved, ctx_.request->Headers());
		if (body.empty()) {
			return kInProgress;
		}
		ctx_.request->SetBody(body.Value());
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
		BodyParser::DestroyBody(request->GetBody());
		utils::DeleteSafe(request);
	}

	IRequest *RequestParser::CopyIRequest(const IRequest *src)
	{
		return CopyRequest(src);
	}

	RequestParser::Request *RequestParser::CopyRequest(const IRequest *src)
	{
		Request *req = new Request();
		req->SetRequestLine(src->GetMessage().GetRequestLine());
		req->SetError(src->GetErrStatusCode(), src->GetErrorType());
		req->SetFieldSection(FieldParser::CopyFieldSection(&src->Headers()));
		req->SetBody(BodyParser::CopyBody(src->GetBody()));
		return req;
	}

} // namespace server
