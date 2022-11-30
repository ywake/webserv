#ifndef BUFFERED_WRITER_HPP
#define BUFFERED_WRITER_HPP

#include "queuing_buffer.hpp"

namespace q_buffer
{
	class BufferedWriter : public virtual QueuingBuffer
	{
	  private:
		std::size_t i;

	  public:
		BufferedWriter();
		BufferedWriter(const BufferedWriter &other);
		BufferedWriter &operator=(const BufferedWriter &rhs);
		Result<void>    Write(int fd);
	};
} // namespace q_buffer

#endif
