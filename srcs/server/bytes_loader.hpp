#ifndef BYTES_LOADER_HPP
#define BYTES_LOADER_HPP

#include <vector>

#include "emptiable.hpp"
#include "queuing_buffer.hpp"

namespace server
{
	class BytesLoader
	{
	  private:
		enum State {
			kStandby,
			kParsing
		};

	  private:
		struct Context {
			std::size_t        max_bytes_;
			std::vector<char> *bytes;
			State              state;
		} ctx_;

	  public:
		BytesLoader();
		BytesLoader(const BytesLoader &other);
		~BytesLoader();
		BytesLoader                   &operator=(const BytesLoader &rhs);
		Emptiable<std::vector<char> *> Parse(q_buffer::QueuingBuffer &recieved);
		bool                           HasInCompleteData();
		void                           SetMaxBytes(std::size_t max_bytes);

	  private:
		void InitParseContext();
		void DestroyParseContext();
	};
} // namespace server

#endif
