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
		ctx_.body = new std::vector<char>();
		*this     = other;
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
		StatefulParser::operator=(rhs);
		max_size_         = rhs.max_size_;
		*ctx_.body        = *rhs.ctx_.body;
		ctx_.state        = rhs.ctx_.state;
		ctx_.chunk_state  = rhs.ctx_.chunk_state;
		ctx_.chunk_size   = rhs.ctx_.chunk_size;
		ctx_.field_parser = rhs.ctx_.field_parser;
		return *this;
	}

	Emptiable<std::vector<char> *> ChunkedParser::Parse(q_buffer::QueuingBuffer &received)
	{
		if (received.empty()) {
			return Emptiable<std::vector<char> *>();
		}
		try {
			if (CreateBody(received) == kDone) {
				std::vector<char> *body = ctx_.body;
				InitParseContext();
				return body;
			}
			return Emptiable<std::vector<char> *>();
		} catch (http::HttpException &e) {
			log("chunked parser", e.what());
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
		ClearLoadedBytes();
		ctx_.body         = new std::vector<char>();
		ctx_.state        = kStandby;
		ctx_.chunk_state  = kChunkSize;
		ctx_.chunk_size   = 0;
		ctx_.field_parser = FieldParser();
	}

	void ChunkedParser::DestroyParseContext()
	{
		utils::DeleteSafe(ctx_.body);
		InitParseContext();
	}

	StatefulParser::ParseResult ChunkedParser::CreateBody(q_buffer::QueuingBuffer &received)
	{
		while (!received.empty()) {
			switch (ParseEachPhase(received)) {
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

	StatefulParser::ParseResult ChunkedParser::ParseEachPhase(q_buffer::QueuingBuffer &received)
	{
		switch (ctx_.state) {
		case kStandby:
			return kDone;
		case kChunk:
			log("chunk");
			return ParseChunked(received);
		case kTrailer:
			log("trailer");
			return DiscardTrailer(received);
		default:
			DBG_INFO;
			throw std::logic_error("chunked parser logic error");
		}
	}

	StatefulParser::ParseResult ChunkedParser::ParseChunked(q_buffer::QueuingBuffer &received)
	{
		while (!received.empty()) {
			switch (ParseEachChunkPhase(received)) {
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

	StatefulParser::ParseResult ChunkedParser::ParseEachChunkPhase(q_buffer::QueuingBuffer &received
	)
	{
		switch (ctx_.chunk_state) {
		case kChunkSize:
			return ParseChunkSize(received);
		case kChunkData:
			return ParseChunkData(received);
		default:
			DBG_INFO;
			throw std::logic_error("chunked parser logic error");
		}
	}

	StatefulParser::ParseResult ChunkedParser::ParseChunkSize(q_buffer::QueuingBuffer &received)
	{
		static const std::size_t kSizeMax  = ~static_cast<std::size_t>(0);
		static const std::size_t kMaxDigit = utils::ToString(kSizeMax).size();

		switch (LoadBytesWithDelim(received, http::kCrLf, kMaxDigit + http::kCrLf.size())) {
		case kOverMaxSize:
			throw http::BadRequestException();
		case kParsable: {
			loaded_bytes_.erase(loaded_bytes_.size() - http::kCrLf.size());
			Result<std::size_t> size = http::ParseChunkSize(loaded_bytes_);
			if (size.IsErr()) {
				throw http::BadRequestException();
			}
			if (ctx_.body->size() > max_size_ || size.Val() > max_size_ - ctx_.body->size()) {
				throw http::ContentTooLargeException();
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

	StatefulParser::ParseResult ChunkedParser::ParseChunkData(q_buffer::QueuingBuffer &received)
	{
		if (ctx_.chunk_size == 0) {
			return kDone;
		}
		std::size_t line_size = ctx_.chunk_size + http::kCrLf.size();
		for (; loaded_bytes_.size() < line_size && !received.empty();) {
			Emptiable<char> c = received.PopChar();
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

	StatefulParser::ParseResult ChunkedParser::DiscardTrailer(q_buffer::QueuingBuffer &received)
	{
		Emptiable<http::FieldSection *> fields = ctx_.field_parser.Parse(received);
		if (fields.empty()) {
			return kInProgress;
		}
		log("trailer done", *fields.Value(), "\n");
		delete fields.Value();
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
