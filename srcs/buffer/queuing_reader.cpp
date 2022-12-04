#include <unistd.h>

#include "queuing_reader.hpp"

namespace q_buffer
{
	const std::size_t QueuingReader::kDefaultReadBufSize = 1024;

	QueuingReader::QueuingReader(std::size_t read_buf_size)
		: QueuingBuffer(), read_buf_size_(read_buf_size)
	{}

	QueuingReader::QueuingReader(const QueuingReader &other) : QueuingBuffer()
	{
		*this = other;
	}

	QueuingReader &QueuingReader::operator=(const QueuingReader &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		QueuingBuffer::operator=(rhs);
		read_buf_size_ = rhs.read_buf_size_;
		return *this;
	}

	ssize_t QueuingReader::Read(int fd)
	{
		buf_.push_back(ByteArray(read_buf_size_));
		void   *buf       = buf_.back().data();
		ssize_t read_size = read(fd, buf, read_buf_size_);
		if (read_size > 0) {
			buf_.back().resize(read_size);
			size_ += read_size;
		} else {
			buf_.pop_back();
		}
		return read_size;
	}
} // namespace q_buffer
