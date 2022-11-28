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
			SetStateAndClearLoadedBytes(kMethod);
			/* Falls through. */
		case kMethod:
		case kTarget:
		case kVersion:
			ParseStartLine(recieved);
			return kInComplete;
		case kHeader:
			return ParseHeaderSection(recieved);
		case kBody:
			return ParseBody(recieved);
		}
		return kInComplete;
	}

	void RequestParser::ParseStartLine(buffer::QueuingBuffer &recieved)
	{
		switch (ctx_.state) {
		case kMethod:
			ParseMethod(recieved);
			if (recieved.empty()) {
				return;
			}
			/* Falls through. */
		case kTarget:
			ParseRequestTarget(recieved);
			if (recieved.empty()) {
				return;
			}
			/* Falls through. */
		case kVersion:
			ParseHttpVersion(recieved);
			/* Falls through. */
		default:
			return;
		}
	}

	void RequestParser::ParseMethod(buffer::QueuingBuffer &recieved)
	{
		LoadResult res = LoadUntillDelim(recieved, http::kSp, http::MethodPool::kMaxMethodLength);
		switch (res) {
		case kParsable:
			break;
		case kNonParsable:
			return;
		case kOverMaxSize:
			throw http::BadRequestException();
		}
		ctx_.loaded_bytes.erase(ctx_.loaded_bytes.size() - http::kSp.size());
		if (!http::abnf::IsMethod(ctx_.loaded_bytes)) {
			throw http::BadRequestException();
		}
		if (!http::MethodPool::Contains(ctx_.loaded_bytes)) {
			// TODO throw NotIMplemented
		}
		ctx_.request->SetMethod(ctx_.loaded_bytes);
		SetStateAndClearLoadedBytes(kTarget);
	}

	void RequestParser::ParseRequestTarget(buffer::QueuingBuffer &recieved)
	{
		LoadResult res = LoadUntillDelim(recieved, http::kSp, kMaxRequestTargetSize);
		switch (res) {
		case kParsable:
			break;
		case kNonParsable:
			return;
		case kOverMaxSize:
			// TODO uri TOOLONG
			throw http::BadRequestException();
		}
		ctx_.loaded_bytes.erase(ctx_.loaded_bytes.size() - http::kSp.size());
		ctx_.request->SetRequestTarget(TryConstructRequestTarget(ctx_.loaded_bytes));
		SetStateAndClearLoadedBytes(kVersion);
	}

	void RequestParser::ParseHttpVersion(buffer::QueuingBuffer &recieved)
	{

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
		LoadResult res = LoadUntillDelim(recieved, http::kEmptyLine, kMaxHeaderSectonSize);
		switch (res) {
		case kParsable:
			break;
		case kNonParsable:
			return kInComplete;
		case kOverMaxSize:
			throw http::BadRequestException();
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

	RequestParser::LoadResult RequestParser::LoadUntillDelim(
		buffer::QueuingBuffer &recieved, const std::string &delim, std::size_t max_bytes
	)
	{
		for (;;) {
			if (recieved.empty()) {
				return kNonParsable;
			}
			Emptiable<char> c = recieved.PopChar();
			ctx_.loaded_bytes += c.Value();
			if (ctx_.loaded_bytes.size() > max_bytes) {
				return kOverMaxSize;
			}
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
