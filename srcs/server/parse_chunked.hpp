#ifndef PARSE_CHUNKED_HPP
#define PARSE_CHUNKED_HPP

#include "result.hpp"

namespace http
{
	Result<std::size_t> ParseChunkSize(const std::string &chunk_size);
}
#endif
