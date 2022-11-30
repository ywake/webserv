#include <unistd.h>

#include "buffered_reader.hpp"

namespace q_buffer
{
	const std::size_t BufferedReader::kDefaultBufferSize = 1024;

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
