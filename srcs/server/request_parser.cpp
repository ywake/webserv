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
	const std::size_t RequestParser::kMaxRequestTargetSize = 8196;
	const std::size_t RequestParser::kMaxHeaderSectonSize  = 8196 * 4;

	RequestParser::RequestParser()
	{
		InitParseContext();
	}

	RequestParser::RequestParser(const RequestParser &other) : StatefulParser(other)
	{
		ctx_.state    = other.ctx_.state;
		*ctx_.request = *other.ctx_.request;
	}

	RequestParser &RequestParser::operator=(const RequestParser &rhs)
	{
		if (this == &rhs) {
			return *this;
		}
		StatefulParser::operator=(rhs);
		ctx_.state    = rhs.ctx_.state;
		*ctx_.request = *rhs.ctx_.request;
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
		ctx_.state   = kStandBy;
		ctx_.request = new Request();
	}

	Emptiable<IRequest *> RequestParser::Parse(buffer::QueuingBuffer &recieved)
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

	RequestParser::ParseResult RequestParser::CreateRequestMessage(buffer::QueuingBuffer &recieved)
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
	RequestParser::ParseResult RequestParser::ParseEachPhase(buffer::QueuingBuffer &recieved)
	{
		switch (ctx_.state) {
		case kStandBy:
			return kInProgress;
		case kMethod:
		case kTarget:
		case kVersion:
			return ParseStartLine(recieved);
		case kHeader:
			return ParseHeaderSection(recieved);
		case kBody:
			return ParseBody(recieved);
		}
		return kInProgress;
	}

	RequestParser::ParseResult RequestParser::ParseStartLine(buffer::QueuingBuffer &recieved)
	{
		switch (ctx_.state) {
		case kMethod:
			return ParseMethod(recieved);
		case kTarget:
			return ParseRequestTarget(recieved);
		case kVersion:
			return ParseHttpVersion(recieved);
		default:
			return kInProgress;
		}
	}

	RequestParser::ParseResult RequestParser::ParseMethod(buffer::QueuingBuffer &recieved)
	{
		switch (LoadBytesWithDelim(recieved, http::kSp, http::MethodPool::kMaxMethodLength)) {
		case kOverMaxSize:
			throw http::NotImplementedException();
		case kParsable:
			loaded_bytes_.erase(loaded_bytes_.size() - http::kSp.size());
			if (!http::abnf::IsMethod(loaded_bytes_)) {
				throw http::BadRequestException();
			}
			if (!http::MethodPool::Contains(loaded_bytes_)) {
				throw http::NotImplementedException();
			}
			ctx_.request->SetMethod(loaded_bytes_);
			return kDone;
		case kNonParsable:
			return kInProgress;
		}
		return kInProgress;
	}

	RequestParser::ParseResult RequestParser::ParseRequestTarget(buffer::QueuingBuffer &recieved)
	{
		switch (LoadBytesWithDelim(recieved, http::kSp, kMaxRequestTargetSize)) {
		case kOverMaxSize:
			throw http::UriTooLongException();
		case kParsable:
			loaded_bytes_.erase(loaded_bytes_.size() - http::kSp.size());
			ctx_.request->SetRequestTarget(TryConstructRequestTarget(loaded_bytes_));
			return kDone;
		case kNonParsable:
			return kInProgress;
		}
		return kInProgress;
	}

	RequestParser::ParseResult RequestParser::ParseHttpVersion(buffer::QueuingBuffer &recieved)
	{
		switch (LoadBytesWithDelim(recieved, http::kCrLf, http::kHttpVersion.size())) {
		case kOverMaxSize:
			throw http::BadRequestException();
		case kParsable:
			loaded_bytes_.erase(loaded_bytes_.size() - http::kCrLf.size());
			if (!http::abnf::IsHttpVersion(loaded_bytes_)) {
				throw http::BadRequestException();
			}
			ctx_.request->SetHttpVersion(loaded_bytes_);
			return kDone;
		case kNonParsable:
			return kInProgress;
		}
		return kInProgress;
	}

	RequestTarget RequestParser::TryConstructRequestTarget(const ThinString &str)
	{
		const std::string &method = ctx_.request->GetMessage().GetMethod();

		if (str.empty()) {
			throw http::BadRequestException();
		} else if (method == "CONNECT") {
			return RequestTarget(AuthorityForm(str));
		} else if (method == "OPTIONS" && str == "*") {
			return RequestTarget(AsteriskForm(str));
		} else if (str.at(0) == '/') {
			return RequestTarget(OriginForm(str));
		} else {
			return RequestTarget(AbsoluteForm(str));
		}
	}

	RequestParser::ParseResult RequestParser::ParseHeaderSection(buffer::QueuingBuffer &recieved)
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
	RequestParser::ParseResult RequestParser::ParseBody(buffer::QueuingBuffer &recieved)
	{
		(void)recieved;
		return kDone;
	}

	RequestParser::State RequestParser::GetNextState(State old_state)
	{
		switch (old_state) {
		case kStandBy:
			return kMethod;
		case kMethod:
			return kTarget;
		case kTarget:
			return kVersion;
		case kVersion:
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
