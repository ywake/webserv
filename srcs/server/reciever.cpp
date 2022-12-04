#include <cerrno>
#include <cstdio>

#include "debug.hpp"
#include "reciever.hpp"
namespace server
{
	Reciever::Reciever(int fd, std::size_t recv_buf_size)
		: QueuingReader(recv_buf_size), fd_(fd), is_eof_(false)
	{}

	Result<void> Reciever::Recv()
	{
		if (IsEof()) {
			return Result<void>();
		}
		ssize_t read_size = Read(fd_);
		log("recv size", read_size);
		log("q_buff size", size());
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
