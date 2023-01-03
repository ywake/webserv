#ifndef FIELD_PARSER_HPP
#define FIELD_PARSER_HPP

#include "field_section.hpp"
#include "stateful_parser.hpp"

namespace server
{
	class FieldParser : public StatefulParser
	{
	  private:
		enum State {
			kStandby,
			kParsing
		};

	  private:
		static const std::size_t kMaxLineSize;
		static const std::size_t kMaxTotalSize;
		struct Context {
			http::FieldSection *fields;
			State               state;
			std::size_t         total_length;
		} ctx_;

	  public:
		FieldParser();
		FieldParser(const FieldParser &other);
		~FieldParser();
		FieldParser                    &operator=(const FieldParser &rhs);
		Emptiable<http::FieldSection *> Parse(q_buffer::QueuingBuffer &received);
		static void                     DestroyFieldSection(const http::FieldSection *fields);
		static http::FieldSection      *CopyFieldSection(const http::FieldSection *src);

	  private:
		void        InitParseContext();
		ParseResult CreateFieldSection(q_buffer::QueuingBuffer &received);
		LoadResult  LoadFieldLine(q_buffer::QueuingBuffer &received, std::size_t max_bytes);

	  protected:
		void DestroyParseContext();
	};
} // namespace server

#endif
