#ifndef BODY_PARSER_HPP
#define BODY_PARSER_HPP

#include "bytes_loader.hpp"
#include "chunked_parser.hpp"
#include "field_section.hpp"
#include "virtual_server_confs.hpp"

namespace server
{
	class BodyParser
	{
	  private:
		enum State {
			kStandby,
			kParsing
		};
		enum Mode {
			kUninitialized,
			kContentLength,
			kChunked
		};

	  private:
		std::size_t                     max_body_size_;
		const conf::VirtualServerConfs *v_server_confs_;

		struct Context {
			BytesLoader   bytes_loader;
			ChunkedParser chunked_parser;
			Mode          mode;
			State         state;
		} ctx_;

	  public:
		BodyParser(const conf::VirtualServerConfs *v_server_confs = &conf::kEmptyVserverConfs);
		BodyParser(const BodyParser &other);
		BodyParser &operator=(const BodyParser &rhs);
		Emptiable<std::vector<char> *>
					Parse(q_buffer::QueuingBuffer &recieved, http::FieldSection &headers);
		bool        HasInCompleteData();
		static void DestroyBody(const std::vector<char> *body);
		static std::vector<char> *CopyBody(const std::vector<char> *src);

	  private:
		void InitMaxSize(const http::FieldSection &headers);
		void InitMode(const http::FieldSection &headers);
		Emptiable<std::vector<char> *>
			 CreateBody(q_buffer::QueuingBuffer &recieved, http::FieldSection &headers);
		void InitParseContext();
		void DestroyParseContext();
	};
} // namespace server

#endif
