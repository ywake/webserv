#include "request_line_parser.hpp"
#include "debug.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "implemented_methods.hpp"
#include "validate_request_line.hpp"
#include "webserv_utils.hpp"
namespace server
{
	const std::size_t RequestLineParser::kMaxRequestTargetSize = 8196;

	RequestLineParser::RequestLineParser()
	{
		InitParseContext();
	}

	RequestLineParser::RequestLineParser(const RequestLineParser &other) : StatefulParser(other)
	{
		ctx_.state        = other.ctx_.state;
		ctx_.request_line = other.ctx_.request_line;
	}

	RequestLineParser &RequestLineParser::operator=(const RequestLineParser &rhs)
	{
		if (this == &rhs) {
			return *this;
		}
		StatefulParser::operator=(rhs);
		ctx_.state        = rhs.ctx_.state;
		ctx_.request_line = rhs.ctx_.request_line;
		return *this;
	}

	void RequestLineParser::InitParseContext()
	{
		ClearLoadedBytes();
		ctx_.state        = kStandBy;
		ctx_.request_line = RequestLine();
	}

	void RequestLineParser::DestroyParseContext()
	{
		InitParseContext();
	}

	bool RequestLineParser::HasInCompleteData()
	{
		return ctx_.state != kStandBy;
	}

	Emptiable<RequestLine> RequestLineParser::Parse(q_buffer::QueuingBuffer &recieved)
	{
		if (recieved.empty()) {
			return Emptiable<RequestLine>();
		}
		try {
			if (CreateRequestLine(recieved) == kDone) {
				RequestLine req = ctx_.request_line;
				InitParseContext();
				return req;
			}
			return Emptiable<RequestLine>();
		} catch (http::HttpException &e) {
			DestroyParseContext();
			log("start line", "throw");
			throw e;
		}
	}

	RequestLineParser::ParseResult
	RequestLineParser::CreateRequestLine(q_buffer::QueuingBuffer &recieved)
	{
		while (!recieved.empty()) {
			switch (ParseEachPhase(recieved)) {
			case kInProgress:
				return kInProgress;
			case kDone:
				if (ctx_.state == kVersion) {
					return kDone;
				}
				ctx_.state = GetNextState(ctx_.state);
				ClearLoadedBytes();
			}
		}
		return kInProgress;
	}

	RequestLineParser::ParseResult
	RequestLineParser::ParseEachPhase(q_buffer::QueuingBuffer &recieved)
	{
		switch (ctx_.state) {
		case kStandBy:
			return kDone;
		case kMethod:
			return ParseMethod(recieved);
		case kTarget:
			return ParseRequestTarget(recieved);
		case kVersion:
			return ParseHttpVersion(recieved);
		}
		return kInProgress;
	}

	RequestLineParser::ParseResult RequestLineParser::ParseMethod(q_buffer::QueuingBuffer &recieved)
	{
		switch (LoadBytesWithDelim(recieved, http::kSp, http::ImplementedMethods::kMaxLength)) {
		case kOverMaxSize:
			throw http::NotImplementedException();
		case kParsable:
			loaded_bytes_.erase(loaded_bytes_.size() - http::kSp.size());
			if (!http::abnf::IsMethod(loaded_bytes_)) {
				throw http::BadRequestException();
			}
			if (!http::ImplementedMethods::Contains(loaded_bytes_)) {
				throw http::NotImplementedException();
			}
			ctx_.request_line.SetMethod(loaded_bytes_);
			return kDone;
		case kNonParsable:
			if (utils::EndWith(loaded_bytes_, http::kCrLf)) {
				log("method bad req", "throw");
				throw http::BadRequestException();
			}
			return kInProgress;
		}
		return kInProgress;
	}

	RequestLineParser::ParseResult
	RequestLineParser::ParseRequestTarget(q_buffer::QueuingBuffer &recieved)
	{
		switch (LoadBytesWithDelim(recieved, http::kSp, kMaxRequestTargetSize)) {
		case kOverMaxSize:
			throw http::UriTooLongException();
		case kParsable:
			loaded_bytes_.erase(loaded_bytes_.size() - http::kSp.size());
			ctx_.request_line.SetRequestTarget(TryConstructRequestTarget(loaded_bytes_));
			return kDone;
		case kNonParsable:
			if (utils::EndWith(loaded_bytes_, http::kCrLf)) {
				log("uri bad req", "throw");
				throw http::BadRequestException();
			}
			return kInProgress;
		}
		return kInProgress;
	}

	RequestLineParser::ParseResult
	RequestLineParser::ParseHttpVersion(q_buffer::QueuingBuffer &recieved)
	{
		log("version", "in");
		switch (LoadBytesWithDelim(
			recieved, http::kCrLf, http::kHttpVersion.size() + http::kCrLf.size()
		)) {
		case kOverMaxSize:
			throw http::BadRequestException();
		case kParsable:
			loaded_bytes_.erase(loaded_bytes_.size() - http::kCrLf.size());
			if (!http::abnf::IsHttpVersion(loaded_bytes_)) {
				log("version bad req", "throw");
				throw http::BadRequestException();
			}
			ctx_.request_line.SetHttpVersion(loaded_bytes_);
			return kDone;
		case kNonParsable:
			return kInProgress;
		}
		return kInProgress;
	}

	RequestTarget RequestLineParser::TryConstructRequestTarget(const ThinString &str)
	{
		const std::string &method = ctx_.request_line.GetMethod();

		log("try construct", "in");
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

	RequestLineParser::State RequestLineParser::GetNextState(State old_state)
	{
		switch (old_state) {
		case kStandBy:
			return kMethod;
		case kMethod:
			return kTarget;
		case kTarget:
			return kVersion;
		case kVersion:
			return kStandBy;
		}
		return kStandBy;
	}

} // namespace server
