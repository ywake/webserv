#include <cerrno>
#include <cstring>
#include <unistd.h>

#include "queuing_writer.hpp"

namespace q_buffer
{
	QueuingWriter::QueuingWriter() : QueuingBuffer() {}

	QueuingWriter::QueuingWriter(const QueuingWriter &other) : QueuingBuffer()
	{
		*this = other;
	}

	QueuingWriter &QueuingWriter::operator=(const QueuingWriter &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		QueuingBuffer::operator=(rhs);
		return *this;
	}

	Result<void> QueuingWriter::Write(int fd)
	{
		if (QueuingBuffer::empty()) {
			return Result<void>();
		}
		const void *buf        = buf_.front().data() + front_idx_;
		std::size_t size       = buf_.front().size() - front_idx_;
		ssize_t     write_size = write(fd, buf, size);
		if (write_size < 0) {
			return Error(std::string("write: ") + strerror(errno));
		}
		front_idx_ += write_size;
		if (front_idx_ == buf_.front().size()) {
			buf_.pop_front();
			front_idx_ = 0;
		}
		return Result<void>();
	}
} // namespace q_buffer
