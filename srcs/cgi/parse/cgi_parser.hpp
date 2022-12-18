#ifndef PARSER_HPP
#define PARSER_HPP

#include "queuing_reader.hpp"
#include "stateful_parser.hpp"

namespace cgi
{
	class Parser : public q_buffer::QueuingReader, public server::StatefulParser
	{
	  private:
		bool parse_end_;

	  public:
		Parser();
		~Parser();

		ssize_t     Read(int fd);
		bool        IsFinished() const;
		std::string ToString();
	};
} // namespace cgi

#endif
