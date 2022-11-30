#ifndef BUFFERED_READER_HPP
#define BUFFERED_READER_HPP

#include <unistd.h>

#include "queuing_buffer.hpp"

namespace q_buffer
{
	class QueuingReader : public virtual QueuingBuffer
	{
	  private:
		static const std::size_t kDefaultBufferSize;
		size_t                   buffer_size_;

	  public:
		QueuingReader(std::size_t buffer_size = kDefaultBufferSize);
		QueuingReader(const QueuingReader &other);
		QueuingReader &operator=(const QueuingReader &rhs);
		ssize_t        Read(int fd);
	};
} // namespace q_buffer

#endif
