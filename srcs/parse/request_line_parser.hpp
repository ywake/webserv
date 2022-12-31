#ifndef REQUEST_LINE_PARSER_HPP
#define REQUEST_LINE_PARSER_HPP

#include "request_line.hpp"
#include "stateful_parser.hpp"

namespace server
{
	class RequestLineParser : public StatefulParser
	{
	  private:
		enum State {
			kStandBy,
			kMethod,
			kTarget,
			kVersion
		};

	  private:
		static const std::size_t kMaxRequestTargetSize;
		struct Context {
			State       state;
			RequestLine request_line;
		} ctx_;

	  public:
		RequestLineParser();
		RequestLineParser(const RequestLineParser &other);
		RequestLineParser &operator=(const RequestLineParser &rhs);

		Emptiable<RequestLine> Parse(q_buffer::QueuingBuffer &received);
		bool                   HasInCompleteData();

	  private:
		void          InitParseContext();
		void          DestroyParseContext();
		ParseResult   CreateRequestLine(q_buffer::QueuingBuffer &received);
		ParseResult   ParseEachPhase(q_buffer::QueuingBuffer &received);
		ParseResult   ParseMethod(q_buffer::QueuingBuffer &received);
		ParseResult   ParseRequestTarget(q_buffer::QueuingBuffer &received);
		ParseResult   ParseHttpVersion(q_buffer::QueuingBuffer &received);
		RequestTarget TryConstructRequestTarget(const ThinString &str);
		State         GetNextState(State old_state);
		LoadResult TryLoadBytesUntilSpace(q_buffer::QueuingBuffer &received, std::size_t max_bytes);
	};
} // namespace server

#endif
