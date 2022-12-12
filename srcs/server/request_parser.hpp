#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include <deque>
#include <string>
#include <vector>

#include "emptiable.hpp"
#include "header_section.hpp"
#include "i_request.hpp"
#include "queuing_buffer.hpp"
#include "request_line_parser.hpp"
#include "request_message.hpp"
#include "result.hpp"
#include "stateful_parser.hpp"
#include "status_code.hpp"
#include "virtual_server_confs.hpp"
namespace server
{
	class RequestParser : public StatefulParser
	{
	  private:
		class Request : public IRequest
		{
		  private:
			http::RequestMessage     request_msg_;
			const std::vector<char> *body_;
			http::StatusCode         error_code_;
			ErrorType                error_type_;

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
			void SetBody(const std::vector<char> *body);

			const std::string          &Method() const;
			const std::string          &Path() const;
			const HeaderSection        &Headers() const;
			const http::RequestMessage &GetMessage() const;
			const http::StatusCode     &GetErrStatusCode() const;
			const std::vector<char>    *GetBody() const;
			const ErrorType            &GetErrorType() const;
		};

	  private:
		enum State {
			kStandBy,
			kStartLine,
			kHeader,
			kBody
		};

	  private:
		static const std::size_t        kMaxHeaderSectonSize;
		const conf::VirtualServerConfs *config_;
		struct Context {
			State             state;
			RequestLineParser request_line_parser;
			Request          *request;
		} ctx_;

	  public:
		RequestParser(const conf::VirtualServerConfs *config = &conf::kEmptyVserverConfs);
		RequestParser(const RequestParser &other);
		~RequestParser();
		RequestParser        &operator=(const RequestParser &rhs);
		Emptiable<IRequest *> Parse(q_buffer::QueuingBuffer &recieved);
		bool                  HasInCompleteData();
		Emptiable<IRequest *> OnEof();
		static void           DestroyRequest(IRequest *&request);
		static IRequest      *CopyRequest(const IRequest *request);

	  private:
		void        InitParseContext();
		void        DestroyParseContext();
		ParseResult CreateRequestMessage(q_buffer::QueuingBuffer &recieved);
		ParseResult ParseEachPhase(q_buffer::QueuingBuffer &recieved);
		ParseResult ParseStartLine(q_buffer::QueuingBuffer &recieved);
		ParseResult ParseHeaderSection(q_buffer::QueuingBuffer &recieved);
		ParseResult ParseBody(q_buffer::QueuingBuffer &recieved);
		State       GetNextState(State old_state);
		ParseResult SkipPriorCrLf(q_buffer::QueuingBuffer &recieved);
	};
} // namespace server
#endif
