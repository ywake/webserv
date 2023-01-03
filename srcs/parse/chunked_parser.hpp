#ifndef CHUNKED_PARSER_HPP
#define CHUNKED_PARSER_HPP

#include "field_parser.hpp"
#include "stateful_parser.hpp"

namespace server
{
	class ChunkedParser : public StatefulParser
	{
	  private:
		enum State {
			kStandby,
			kChunk,
			kTrailer
		};
		enum ChunkState {
			kChunkSize,
			kChunkData
		};

	  private:
		std::size_t max_size_;
		struct Context {
			std::vector<char> *body;
			State              state;
			ChunkState         chunk_state;
			std::size_t        chunk_size;
			FieldParser        field_parser;
		} ctx_;

	  public:
		ChunkedParser(std::size_t max_size = 0);
		ChunkedParser(const ChunkedParser &other);
		~ChunkedParser();
		ChunkedParser                 &operator=(const ChunkedParser &rhs);
		Emptiable<std::vector<char> *> Parse(q_buffer::QueuingBuffer &received);
		bool                           HasInCompleteData();

	  private:
		void        InitParseContext();
		void        DestroyParseContext();
		ParseResult CreateBody(q_buffer::QueuingBuffer &received);
		ParseResult ParseEachPhase(q_buffer::QueuingBuffer &received);
		ParseResult ParseChunked(q_buffer::QueuingBuffer &received);
		ParseResult ParseEachChunkPhase(q_buffer::QueuingBuffer &received);
		ParseResult ParseChunkSize(q_buffer::QueuingBuffer &received);
		ParseResult ParseChunkData(q_buffer::QueuingBuffer &received);
		ParseResult DiscardTrailer(q_buffer::QueuingBuffer &received);
		State       GetNextState(State old_state);
		ChunkState  GetNextChunkState(ChunkState old_state);
	};
} // namespace server

#endif
