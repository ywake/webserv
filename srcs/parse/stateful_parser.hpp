#ifndef STATEFUL_PARSER_HPP
#define STATEFUL_PARSER_HPP

#include <string>

#include "queuing_buffer.hpp"

namespace server
{
	class StatefulParser
	{
	  protected:
		enum ParseResult {
			kDone,
			kInProgress
		};
		enum LoadResult {
			kParsable,
			kNonParsable,
			kOverMaxSize
		};

	  public:
		StatefulParser();
		StatefulParser(const StatefulParser &other);
		StatefulParser &operator=(const StatefulParser &rhs);

	  protected:
		std::string loaded_bytes_;

	  protected:
		LoadResult LoadBytesWithDelim(
			q_buffer::QueuingBuffer &received, const std::string &delim, std::size_t max_bytes
		);
		void ClearLoadedBytes();
	};

} // namespace server

#endif
