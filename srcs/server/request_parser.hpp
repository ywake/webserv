#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include <deque>

#include "buffer.hpp"
#include "emptiable.hpp"
#include "i_request.hpp"
#include "request_message.hpp"
#include "result.hpp"
#include "status_code.hpp"

namespace server
{
	class RequestParser
	{
	  private:
		class Request : public IRequest
		{
		  public:
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
			void SetError(const http::StatusCode &error_code, ErrorType error_type);
			bool IsErr() const;
			bool IsFatal() const;

			const http::RequestMessage &GetMessage() const;
			const http::StatusCode     &GetErrStatusCode() const;
			const ErrorType            &GetErrorType() const;
		};

	  private:
		enum State {
			kStandBy,
			kStartLine,
			kHeader,
			kBody
		};
		enum ParseResult {
			kComplete,
			kInComplete
		};
		enum LoadResult {
			kParsable,
			kNonParsable
		};

	  private:
		struct Context {
			State       state;
			std::string loaded_bytes;
			Request    *request;
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
		ParseResult ParseStartLine(buffer::QueuingBuffer &recieved);
		ParseResult ParseHeaderSection(buffer::QueuingBuffer &recieved);
		ParseResult ParseBody(buffer::QueuingBuffer &recieved);
		LoadResult  LoadUntillDelim(buffer::QueuingBuffer &recieved, const std::string &delim);
		void        SetStateAndClearLoadedBytes(State new_state);
	};
} // namespace server
#endif
