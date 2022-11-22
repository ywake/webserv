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


	bool Reciever::IsEof()
	{
		return is_eof_;
	}

} // namespace server
