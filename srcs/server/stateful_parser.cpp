#include "stateful_parser.hpp"
#include "webserv_utils.hpp"

namespace server
{
	StatefulParser::StatefulParser() : loaded_bytes_() {}

	StatefulParser::StatefulParser(const StatefulParser &other) : loaded_bytes_(other.loaded_bytes_)
	{}

	StatefulParser &StatefulParser::operator=(const StatefulParser &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		loaded_bytes_ = rhs.loaded_bytes_;
		return *this;
	}

	StatefulParser::LoadResult StatefulParser::LoadBytesWithDelim(
		q_buffer::QueuingBuffer &recieved, const std::string &delim, std::size_t max_bytes
	)
	{
		for (;;) {
			if (recieved.empty()) {
				return kNonParsable;
			}
			Emptiable<char> c = recieved.PopChar();
			loaded_bytes_ += c.Value();
			if (loaded_bytes_.size() > max_bytes) {
				return kOverMaxSize;
			}
			if (utils::EndWith(loaded_bytes_, delim)) {
				return kParsable;
			}
		}
	}

	void StatefulParser::ClearLoadedBytes()
	{
		loaded_bytes_ = std::string();
	}

} // namespace server
