#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include <deque>
#include <string>
#include <vector>

#include "body_parser.hpp"
#include "emptiable.hpp"
#include "field_section.hpp"
#include "header_parser.hpp"
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
			RequestLine              request_line_;
			http::FieldSection      *field_section_;
			const std::vector<char> *body_;
			http::StatusCode         error_code_;
			ErrorType                error_type_;

		  private:
			Request(const Request &other);

		  public:
			Request();
			Request(const http::StatusCode &error_code, ErrorType error_type);
			~Request();

			bool IsErr() const;
			bool IsFatal() const;
			bool HasMessageBody() const;

			void SetError(const http::StatusCode &error_code, ErrorType error_type);
			void SetRequestLine(const RequestLine &request_line);
			void SetFieldSection(http::FieldSection *field_lines);
			void SetBody(const std::vector<char> *body);
			void ReconstructUri();

			const std::string        &Method() const;
			const std::string        &Path() const;
			const std::string        &Host() const;
			const std::string        &Port() const;
			std::string               Authority() const;
			http::FieldSection       &Headers();
			const http::FieldSection &Headers() const;
			const RequestLine        &GetRequestLine() const;
			const http::StatusCode   &GetErrStatusCode() const;
			const std::vector<char>  *GetBody() const;
			const ErrorType          &GetErrorType() const;
			bool                      NeedToClose() const;
			void                      SetPath(const std::string &path);
			void                      SetQuery(const std::string &query);
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
			BodyParser        body_parser;
			HeaderParser      header_parser;
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
		static void           DestroyIRequest(IRequest *&request);
		static IRequest      *CopyIRequest(const IRequest *request);

	  private:
		void            InitParseContext();
		void            DestroyParseContext();
		ParseResult     CreateRequestMessage(q_buffer::QueuingBuffer &recieved);
		ParseResult     ParseEachPhase(q_buffer::QueuingBuffer &recieved);
		ParseResult     ParseStartLine(q_buffer::QueuingBuffer &recieved);
		ParseResult     ParseHeaderSection(q_buffer::QueuingBuffer &recieved);
		ParseResult     ParseBody(q_buffer::QueuingBuffer &recieved);
		State           GetNextState(State old_state);
		ParseResult     SkipPriorCrLf(q_buffer::QueuingBuffer &recieved);
		static void     DestroyRequest(IRequest *request);
		static Request *CopyRequest(const IRequest *src);
	};
} // namespace server
#endif
