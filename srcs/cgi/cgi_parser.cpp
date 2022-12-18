#include "cgi_parser.hpp"

namespace cgi
{
	Parser::Parser() : q_buffer::QueuingReader(), parse_end_(false) {}

	Parser::~Parser() {}

	ssize_t Parser::Read(int fd)
	{
		ssize_t size = q_buffer::QueuingReader::Read(fd);
		if (size == 0) {
			parse_end_ = true;
		}
		return size;
	}

	bool Parser::IsFinished() const
	{
		return parse_end_;
	}

	std::string Parser::ToString()
	{
		std::vector<char> buf = PopAll();
		return std::string(buf.begin(), buf.end());
	}

} // namespace cgi
