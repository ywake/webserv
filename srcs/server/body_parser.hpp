#ifndef BODY_PARSER_HPP
#define BODY_PARSER_HPP

#include "stateful_parser.hpp"

namespace server
{
	class BodyParser : public StatefulParser
	{
	  private:
		enum Mode {
			kStandby,
			kContentLength,
			kChunked
		};
		enum State {
			kChunkSize,
			kChunkData
		};

	  private:
		struct Context {
			std::vector<char> *body;
			Mode               mode;
			State              state;
		} ctx_;

	  public:
		BodyParser();
		BodyParser(const BodyParser &other);
		~BodyParser();
		BodyParser                    &operator=(const BodyParser &rhs);
		Emptiable<std::vector<char> *> Parse(q_buffer::QueuingBuffer &recieved);
		bool                           HasInCompleteData();

	  private:
		void        InitParseContext();
		void        DestroyParseContext();
		ParseResult CreateBody(q_buffer::QueuingBuffer &recieved);
		ParseResult ParseChunked(q_buffer::QueuingBuffer &recieved);
	};
} // namespace server

#endif
