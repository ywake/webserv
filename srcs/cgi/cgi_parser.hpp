#ifndef CGI_PARSER_HPP
#define CGI_PARSER_HPP

#include "field_section.hpp"
#include "stateful_parser.hpp"

namespace cgi
{
	class CgiParser : public server::StatefulParser
	{
	  private:
		enum State {
			kStandby,
			kParsing
		};

	  private:
		static const std::string kCr;
		static const std::string kLf;
		static const std::size_t kMaxLineSize;
		static const std::size_t kMaxTotalSize;
		struct Context {
			http::FieldSection *fields;
			State               state;
			std::size_t         total_length;
		} ctx_;

	  public:
		CgiParser();
		CgiParser(const CgiParser &other);
		~CgiParser();
		CgiParser                      &operator=(const CgiParser &rhs);
		Emptiable<http::FieldSection *> Parse(q_buffer::QueuingBuffer &received);
		static void                     DestroyFieldSection(const http::FieldSection *fields);
		static http::FieldSection      *CopyFieldSection(const http::FieldSection *src);

	  private:
		void        InitParseContext();
		ParseResult CreateFieldSection(q_buffer::QueuingBuffer &received);
		void        EraseLfOrCrLf(std::string &field_line);

	  protected:
		void DestroyParseContext();
	};
} // namespace cgi

#endif
