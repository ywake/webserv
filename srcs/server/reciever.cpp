#include <cerrno>
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>

#include "reciever.hpp"

namespace server
{
	Reciever::Reciever(int fd, std::size_t buffer_size)
		: fd_(fd), buffer_size_(buffer_size), is_eof_(false)
	{}

	Result<void> Reciever::Recv()
	{
		buf_.push_back(ByteArray(buffer_size_));
		void   *buf       = buf_.back().data();
		ssize_t recv_size = recv(fd_, buf, buffer_size_, 0);
		if (recv_size > 0) {
			buf_.back().resize(recv_size);
			return Result<void>();
		}
		buf_.pop_back();
		if (recv_size == 0) {
			is_eof_ = true;
			return Result<void>();
		}
		return Error(std::string("recv: ") + strerror(errno));
	}

	bool Reciever::IsEof()
	{
		return is_eof_;
	}

} // namespace server
