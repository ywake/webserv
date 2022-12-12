#include <cerrno>
#include <cstdio>
#include <stdexcept>
#include <unistd.h>

#include "body_writer.hpp"
#include "debug.hpp"

namespace server
{
	const std::size_t BodyWriter::kDefaultChunkSize = 8192;

	BodyWriter::BodyWriter(const std::vector<char> *body, std::size_t chunk_size_)
		: body_(body), chunk_size_(chunk_size_), used_size_()
	{}

	BodyWriter::BodyWriter(const BodyWriter &other)
	{
		*this = other;
	}

	BodyWriter &BodyWriter::operator=(const BodyWriter &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		body_       = rhs.body_;
		chunk_size_ = rhs.chunk_size_;
		used_size_  = rhs.used_size_;
		return *this;
	}

	Result<void> BodyWriter::Write(int fd)
	{
		if (body_ == NULL || used_size_ > body_->size()) {
			DBG_INFO;
			throw std::logic_error("body write error");
		}
		std::size_t size       = std::min(chunk_size_, body_->size() - used_size_);
		ssize_t     write_size = write(fd, body_->data() + used_size_, size);
		if (write_size < 0) {
			return Error("body write: " + std::string(strerror(errno)));
		}
		used_size_ += write_size;
		return Result<void>();
	}

	bool BodyWriter::IsFinished() const
	{
		return body_->size() == used_size_;
	}
} // namespace server
