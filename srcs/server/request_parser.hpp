#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include <deque>

#include "buffer.hpp"
#include "emptiable.hpp"
#include "i_request.hpp"
#include "request_message.hpp"
#include "result.hpp"
#include "stateful_parser.hpp"
#include "status_code.hpp"

namespace server
{
	class RequestParser : public StatefulParser
	{
	  private:
		class Request : public IRequest
		{
		  private:
			http::RequestMessage request_msg_;
			http::StatusCode     error_code_;
			ErrorType            error_type_;

		  public:
			Request(
				const http::RequestMessage &request_msg = http::RequestMessage(),
				const http::StatusCode     &error_code  = http::StatusCode(),
				const ErrorType            &error_type  = kNotError
			);
			Request(const Request &other);
			Request(const http::StatusCode &error_code, ErrorType error_type);
			~Request();

			bool IsErr() const;
			bool IsFatal() const;
			bool HasMessageBody() const;

			void SetError(const http::StatusCode &error_code, ErrorType error_type);
			void SetRequestLine(const RequestLine &request_line);
			void SetHeaderSection(const HeaderSection &field_lines);
			void SetBody(const std::string &body);

			const http::RequestMessage &GetMessage() const;
			const http::StatusCode     &GetErrStatusCode() const;
			const ErrorType            &GetErrorType() const;
		};

	  private:
		enum State {
			kStandBy,
			kMethod,
			kTarget,
			kVersion,
			kHeader,
			kBody
		};

	  private:
		static const std::size_t kMaxRequestTargetSize;
		static const std::size_t kMaxHeaderSectonSize;
		struct Context {
			State    state;
			Request *request;
		} ctx_;

	  public:
		RequestParser();
		RequestParser(const RequestParser &other);
		~RequestParser();
		RequestParser        &operator=(const RequestParser &rhs);
		Emptiable<IRequest *> Parse(buffer::QueuingBuffer &recieved);
		bool                  HasInCompleteData();
		Emptiable<IRequest *> OnEof();
		void                  DestroyParseContext();
		static void           DestroyRequest(IRequest *&request);
		static IRequest      *CopyRequest(const IRequest *request);

	  private:
		void        InitParseContext();
		ParseResult CreateRequestMessage(buffer::QueuingBuffer &recieved);

		ParseResult   ParseEachPhase(buffer::QueuingBuffer &recieved);
		ParseResult   ParseStartLine(buffer::QueuingBuffer &recieved);
		ParseResult   ParseMethod(buffer::QueuingBuffer &recieved);
		ParseResult   ParseRequestTarget(buffer::QueuingBuffer &recieved);
		ParseResult   ParseHttpVersion(buffer::QueuingBuffer &recieved);
		RequestTarget TryConstructRequestTarget(const ThinString &str);

		ParseResult ParseHeaderSection(buffer::QueuingBuffer &recieved);
		ParseResult ParseBody(buffer::QueuingBuffer &recieved);
		State       GetNextState(State old_state);
	};
} // namespace server
#endif
