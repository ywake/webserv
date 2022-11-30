#include <cerrno>
#include <cstdio>

#include "reciever.hpp"

namespace server
{
	Reciever::Reciever(int fd, std::size_t buffer_size)
		: fd_(fd), buffer_size_(buffer_size), is_eof_(false)
	{}

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
