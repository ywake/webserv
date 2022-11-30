#ifndef BUFFERED_READER_HPP
#define BUFFERED_READER_HPP

#include <unistd.h>

#include "queuing_buffer.hpp"

namespace q_buffer
{
	class BufferedReader : public virtual QueuingBuffer
	{
	  private:
		static const std::size_t kDefaultBufferSize;
		size_t                   buffer_size_;

	  public:
		BufferedReader(std::size_t buffer_size = kDefaultBufferSize);
		BufferedReader(const BufferedReader &other);
		BufferedReader &operator=(const BufferedReader &other);
		ssize_t         Read(int fd);
	};
} // namespace q_buffer

#endif
