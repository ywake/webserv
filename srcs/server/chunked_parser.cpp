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
