#include <unistd.h>

#include "buffered_reader.hpp"

namespace q_buffer
{
	const std::size_t BufferedReader::kDefaultBufferSize = 1024;

	BufferedReader::BufferedReader(std::size_t buffer_size)
		: QueuingBuffer(), buffer_size_(buffer_size)
	{}

	BufferedReader::BufferedReader(const BufferedReader &other) : QueuingBuffer()
	{
		*this = other;
	}

	BufferedReader &BufferedReader::operator=(const BufferedReader &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		QueuingBuffer::operator=(rhs);
		buffer_size_ = rhs.buffer_size_;
		return *this;
	}

	ssize_t BufferedReader::Read(int fd)
	{
		buf_.push_back(ByteArray(buffer_size_));
		void   *buf       = buf_.back().data();
		ssize_t read_size = read(fd, buf, buffer_size_);
		if (read_size > 0) {
			buf_.back().resize(read_size);
		} else {
			buf_.pop_back();
		}
		return read_size;
	}
} // namespace q_buffer
