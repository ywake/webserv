#include "chunked_parser.hpp"

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
		for (; loaded_bytes_.size() < ctx_.chunk_size && !recieved.empty();) {
			Emptiable<char> c = recieved.PopChar();
			loaded_bytes_ += c.Value();
		}
		if (loaded_bytes_.size() < ctx_.chunk_size) {
			return kInProgress;
		}
		ctx_.body->insert(ctx_.body->end(), loaded_bytes_.begin(), loaded_bytes_.end());
		return kDone;
	}

	StatefulParser::ParseResult ChunkedParser::DiscardTrailer(q_buffer::QueuingBuffer &recieved)
	{
		// TODO
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
