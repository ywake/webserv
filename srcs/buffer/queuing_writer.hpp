#ifndef QUEUING_WRITER_HPP
#define QUEUING_WRITER_HPP

#include "queuing_buffer.hpp"

namespace q_buffer
{
	class QueuingWriter : public virtual QueuingBuffer
	{
	  public:
		QueuingWriter();
		QueuingWriter(const QueuingWriter &other);
		QueuingWriter &operator=(const QueuingWriter &rhs);
		Result<void>   Write(int fd);
	};
} // namespace q_buffer

#endif
