#ifndef BODY_PARSER_HPP
#define BODY_PARSER_HPP

#include "bytes_loader.hpp"
#include "chunked_parser.hpp"
#include "header_section.hpp"
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
		static const conf::VirtualServerConfs kEmptyConfs;
		std::size_t                           max_size_;
		const conf::VirtualServerConfs       *v_server_confs_;

		struct Context {
			BytesLoader   bytes_loader;
			ChunkedParser chunked_parser;
			Mode          mode;
			State         state;
		} ctx_;

	  public:
		BodyParser(const conf::VirtualServerConfs *v_server_confs = &kEmptyConfs);
		BodyParser(const BodyParser &other);
		BodyParser &operator=(const BodyParser &rhs);
		Emptiable<std::vector<char> *>
			 Parse(q_buffer::QueuingBuffer &recieved, const HeaderSection &headers);
		bool HasInCompleteData();
		void DestroyBody(const std::vector<char> *body);

	  private:
		void InitMode(const HeaderSection &headers);
		void InitParseContext();
		void DestroyParseContext();
	};
} // namespace server

#endif
