#include "chunked_parser.hpp"
#include "debug.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "parse_chunked.hpp"
#include "webserv_utils.hpp"

namespace server
{
	ChunkedParser::ChunkedParser(std::size_t max_size) : max_size_(max_size)
	{
		InitParseContext();
	}

	ChunkedParser::ChunkedParser(const ChunkedParser &other) : StatefulParser()
	{
		*this = other;
	}

	ChunkedParser::~ChunkedParser()
	{
		delete ctx_.body;
	}

	ChunkedParser &ChunkedParser::operator=(const ChunkedParser &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		DestroyParseContext();
		max_size_        = rhs.max_size_;
		*ctx_.body       = *rhs.ctx_.body;
		ctx_.state       = rhs.ctx_.state;
		ctx_.chunk_state = rhs.ctx_.chunk_state;
		ctx_.chunk_size  = rhs.ctx_.chunk_size;
		return *this;
	}

	Emptiable<std::vector<char> *> ChunkedParser::Parse(q_buffer::QueuingBuffer &recieved)
	{
		if (recieved.empty()) {
			return Emptiable<std::vector<char> *>();
		}
		try {
			if (CreateBody(recieved) == kDone) {
				std::vector<char> *body = ctx_.body;
				InitParseContext();
				return body;
			}
			return Emptiable<std::vector<char> *>();
		} catch (http::HttpException &e) {
			DestroyParseContext();
			throw e;
		}
	}

	bool ChunkedParser::HasInCompleteData()
	{
		return ctx_.state != kStandby;
	}

	void ChunkedParser::InitParseContext()
	{
		ctx_.body       = new std::vector<char>();
		ctx_.state      = kStandby;
		ctx_.chunk_size = 0;
	}

	void ChunkedParser::DestroyParseContext()
	{
		utils::DeleteSafe(ctx_.body);
		InitParseContext();
	}

	StatefulParser::ParseResult ChunkedParser::CreateBody(q_buffer::QueuingBuffer &recieved)
	{
		while (!recieved.empty()) {
			switch (ParseEachPhase(recieved)) {
			case kInProgress:
				return kInProgress;
			case kDone:
				if (ctx_.state == kTrailer) {
					return kDone;
				}
				ctx_.state = GetNextState(ctx_.state);
				ClearLoadedBytes();
				log("chunked next parse state");
			}
		}
		return kInProgress;
	}

	StatefulParser::ParseResult ChunkedParser::ParseEachPhase(q_buffer::QueuingBuffer &recieved)
	{
		switch (ctx_.state) {
		case kStandby:
			return kDone;
		case kChunk:
			log("chunk");
			return ParseChunked(recieved);
		case kTrailer:
			log("trailer");
			return DiscardTrailer(recieved);
		default:
			DBG_INFO;
			throw std::logic_error("chunked parser logic error");
		}
	}

	StatefulParser::ParseResult ChunkedParser::ParseChunked(q_buffer::QueuingBuffer &recieved)
	{
		while (!recieved.empty()) {
			switch (ParseEachChunkPhase(recieved)) {
			case kInProgress:
				return kInProgress;
			case kDone:
				if (ctx_.chunk_state == kChunkData && ctx_.chunk_size == 0) {
					return kDone;
				}
				ctx_.chunk_state = GetNextChunkState(ctx_.chunk_state);
				ClearLoadedBytes();
				log("chunked next parse state");
			}
		}
		return kInProgress;
	}

	StatefulParser::ParseResult ChunkedParser::ParseEachChunkPhase(q_buffer::QueuingBuffer &recieved
	)
	{
		switch (ctx_.chunk_state) {
		case kChunkSize:
			return ParseChunkSize(recieved);
		case kChunkData:
			return ParseChunkData(recieved);
		default:
			DBG_INFO;
			throw std::logic_error("chunked parser logic error");
		}
	}

	StatefulParser::ParseResult ChunkedParser::ParseChunkSize(q_buffer::QueuingBuffer &recieved)
	{
		switch (LoadBytesWithDelim(recieved, http::kCrLf, max_size_)) {
		case kOverMaxSize:
			throw http::ContentTooLargeException();
		case kParsable: {
			loaded_bytes_.erase(loaded_bytes_.size() - http::kCrLf.size());
			Result<std::size_t> size = http::ParseChunkSize(loaded_bytes_);
			if (size.IsErr()) {
				throw http::BadRequestException();
			}
			ctx_.chunk_size = size.Val();
			return kDone;
		}
		case kNonParsable:
			return kInProgress;
		default:
			DBG_INFO;
			throw std::logic_error("chunk size parser logic error");
		}
	}

	StatefulParser::ParseResult ChunkedParser::ParseChunkData(q_buffer::QueuingBuffer &recieved)
	{
		if (ctx_.chunk_size == 0) {
			return kDone;
		}
		std::size_t line_size = ctx_.chunk_size + http::kCrLf.size();
		for (; loaded_bytes_.size() < line_size && !recieved.empty();) {
			Emptiable<char> c = recieved.PopChar();
			loaded_bytes_ += c.Value();
		}
		if (loaded_bytes_.size() < line_size) {
			return kInProgress;
		}
		if (!utils::EndWith(loaded_bytes_, http::kCrLf)) {
			throw http::BadRequestException();
		}
		loaded_bytes_.erase(loaded_bytes_.size() - http::kCrLf.size());
		ctx_.body->insert(ctx_.body->end(), loaded_bytes_.begin(), loaded_bytes_.end());
		return kDone;
	}

	StatefulParser::ParseResult ChunkedParser::DiscardTrailer(q_buffer::QueuingBuffer &recieved)
	{
		// TODO
		(void)recieved;
		return kDone;
	}

	ChunkedParser::State ChunkedParser::GetNextState(State old_state)
	{
		switch (old_state) {
		case kStandby:
			return kChunk;
		case kChunk:
			return kTrailer;
		case kTrailer:
			return kStandby;
		default:
			DBG_INFO;
			throw std::logic_error("chunked parser logic error");
		}
	}

	ChunkedParser::ChunkState ChunkedParser::GetNextChunkState(ChunkState old_state)
	{
		switch (old_state) {
		case kChunkSize:
			return kChunkData;
		case kChunkData:
			return kChunkSize;
		default:
			DBG_INFO;
			throw std::logic_error("chunked parser logic error");
		}
	}

} // namespace server
