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

} // namespace server
