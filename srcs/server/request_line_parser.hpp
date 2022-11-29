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

		Emptiable<RequestLine> Parse(buffer::QueuingBuffer &recieved);
		bool                   HasInCompleteData();

	  private:
		void          InitParseContext();
		void          DestroyParseContext();
		ParseResult   CreateRequestLine(buffer::QueuingBuffer &recieved);
		ParseResult   ParseEachPhase(buffer::QueuingBuffer &recieved);
		ParseResult   ParseMethod(buffer::QueuingBuffer &recieved);
		ParseResult   ParseRequestTarget(buffer::QueuingBuffer &recieved);
		ParseResult   ParseHttpVersion(buffer::QueuingBuffer &recieved);
		RequestTarget TryConstructRequestTarget(const ThinString &str);
		State         GetNextState(State old_state);
	};
} // namespace server

#endif
