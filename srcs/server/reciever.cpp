#include <cerrno>
#include <cstdio>

#include "reciever.hpp"

namespace server
{
	Reciever::Reciever(int fd, std::size_t recv_buf_size)
		: QueuingReader(recv_buf_size), fd_(fd), is_eof_(false)
	{}

	Reciever::Reciever(const Reciever &other) : QueuingBuffer(), QueuingReader()
	{
		*this = other;
	}

	Reciever &Reciever::operator=(const Reciever &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		QueuingReader::operator=(rhs);
		fd_     = rhs.fd_;
		is_eof_ = rhs.is_eof_;
		return *this;
	}

	Result<void> Reciever::Recv()
	{
		if (IsEof()) {
			return Result<void>();
		}
		ssize_t read_size = Read(fd_);
		if (read_size < 0) {
			return Error(std::string("recv: ") + strerror(errno));
		}
		is_eof_ = read_size == 0;
		return Result<void>();
	}

	bool Reciever::IsEof()
	{
		return is_eof_;
	}

} // namespace server
